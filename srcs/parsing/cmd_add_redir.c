/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_add_cmd_redir(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !token->str || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	var_cmd->file_temp = ft_strdup(token->str); // dupliquer le nom du fichier (redirection >) pour stocker dans cmd
	if (!var_cmd->file_temp)
		return (-1);
	var_cmd->size_file_tab = len_tab_char(cmd[var_cmd->index_cmd].inoutfile); // compter la taille actuelle du tableau outfile
	var_cmd->new_tab_char = add_double_tab_char(cmd[var_cmd->index_cmd].inoutfile, var_cmd->file_temp, var_cmd->size_file_tab); // agrandir le tableau outfile pour ajouter le nouveau fichier
	if (!var_cmd->new_tab_char)
		return (free(var_cmd->file_temp), var_cmd->file_temp = NULL, -1);
	cmd[var_cmd->index_cmd].inoutfile = var_cmd->new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
	if (token->type_token == T_FD_IN)
		var_cmd->new_tab_int = add_double_tab_int(cmd[var_cmd->index_cmd].ihoa, 3, var_cmd->size_file_tab);
	else if (token->type_token == T_FD_OUT)
		var_cmd->new_tab_int = add_double_tab_int(cmd[var_cmd->index_cmd].ihoa, 1, var_cmd->size_file_tab); // agrandir le tableau out_append pour ajouter 0 (truncate) <- puisqu'on est dans la condition T_FD_OUT >
	// 0='>' (truncate), 1='>>' (append)
	else if (token->type_token == T_FD_OUT_APPEND)
		var_cmd->new_tab_int = add_double_tab_int(cmd[var_cmd->index_cmd].ihoa, 2, var_cmd->size_file_tab);
	else
		return (-1);
	if (!var_cmd->new_tab_int)
		return (var_cmd->new_tab_int = NULL, -1);
	cmd[var_cmd->index_cmd].ihoa = var_cmd->new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
	cmd[var_cmd->index_cmd].compter_ihoa++;
	return (0);
}

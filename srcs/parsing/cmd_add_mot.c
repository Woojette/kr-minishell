/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add_mot.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_add_cmd_mot_1(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	if (cmd[var_cmd->index_cmd].cmd == NULL)
	{
		cmd[var_cmd->index_cmd].cmd = malloc(sizeof(char *) * 2);
		if (!cmd[var_cmd->index_cmd].cmd)
			return (-1);
		cmd[var_cmd->index_cmd].cmd[0] = ft_strdup(token->str);
		if (!cmd[var_cmd->index_cmd].cmd[0])
			return (free(cmd[var_cmd->index_cmd].cmd),
				cmd[var_cmd->index_cmd].cmd = NULL, -1);
		cmd[var_cmd->index_cmd].cmd[1] = NULL;
	}
	else if (cmd[var_cmd->index_cmd].cmd)
		return (1);
	return (0);
}

int	appliquer_add_cmd_mot_2(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	if (!cmd[var_cmd->index_cmd].cmd)
		return (-1);
	var_cmd->mot_temp = ft_strdup(token->str);
	if (!var_cmd->mot_temp)
		return (-1);
	var_cmd->size_cmd = len_tab_char(cmd[var_cmd->index_cmd].cmd);
	var_cmd->new_tab_char = add_double_tab_char(cmd[var_cmd->index_cmd].cmd,
			var_cmd->mot_temp, var_cmd->size_cmd);
	if (!var_cmd->new_tab_char)
		return (free(var_cmd->mot_temp), var_cmd->mot_temp = NULL, -1);
	cmd[var_cmd->index_cmd].cmd = var_cmd->new_tab_char;
	return (0);
}

int	appliquer_add_cmd_mot_all(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	int	resultat_1;

	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	resultat_1 = appliquer_add_cmd_mot_1(token, cmd, var_cmd);
	if (resultat_1 < 0)
		return (-1);
	else if (resultat_1 == 1)
	{
		if (appliquer_add_cmd_mot_2(token, cmd, var_cmd) < 0)
			return (-1);
	}
	var_cmd->i++;
	return (0);
}

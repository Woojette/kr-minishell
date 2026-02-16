/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_add_cmd_pipe(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	char	*erreur;

	erreur = "Error: syntax error near unexpected token `|'\n";
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	if (cmd[var_cmd->index_cmd].inoutfile
		|| cmd[var_cmd->index_cmd].compter_ihoa > 0)
		var_cmd->redir_existe = 1;
	else
		var_cmd->redir_existe = 0;
	if (token->next == NULL)
		return (write(2, erreur, ft_strlen(erreur)), -2);
	if (token->next->type_token == T_PIPE)
		return (write(2, erreur, ft_strlen(erreur)), -2);
	if (cmd[var_cmd->index_cmd].cmd == NULL && var_cmd->i == 0
		&& !var_cmd->redir_existe)
		return (write(2, erreur, ft_strlen(erreur)), -2);
	if (cmd[var_cmd->index_cmd].cmd != NULL)
		cmd[var_cmd->index_cmd].cmd[var_cmd->i] = NULL;
	var_cmd->index_cmd++;
	var_cmd->i = 0;
	return (0);
}

int	appliquer_add_cmd_type(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	if (token->type_token == T_MOT)
		return (appliquer_add_cmd_mot_all(token, cmd, var_cmd));
	if (token->type_token == T_FD_IN || token->type_token == T_FD_OUT
		|| token->type_token == T_FD_OUT_APPEND)
		return (appliquer_add_cmd_redir(token, cmd, var_cmd));
	if (token->type_token == T_FD_HEREDOC)
		return (appliquer_add_cmd_heredoc_all(token, cmd, var_cmd));
	if (token->type_token == T_PIPE)
		return (appliquer_add_cmd_pipe(token, cmd, var_cmd));
	return (0);
}

int	add_cmd(t_token *token, t_cmd *cmd)
{
	int			resultat;
	t_var_cmd	var_cmd;

	resultat = 0;
	init_var_cmd(&var_cmd, &resultat);
	while (token)
	{
		resultat = appliquer_add_cmd_type(token, cmd, &var_cmd);
		if (resultat < 0)
			return (free_cmd_interieur(cmd, var_cmd.index_cmd + 1), resultat);
		token = token->next;
	}
	if (var_cmd.index_cmd > 0 && cmd[var_cmd.index_cmd].cmd == NULL
		&& cmd[var_cmd.index_cmd].inoutfile == NULL
		&& !cmd[var_cmd.index_cmd].heredoc)
	{
		free_cmd_interieur(cmd, var_cmd.index_cmd + 1);
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47),
			-2);
	}
	if (cmd[var_cmd.index_cmd].cmd != NULL)
		cmd[var_cmd.index_cmd].cmd[var_cmd.i] = NULL;
	return (0);
}

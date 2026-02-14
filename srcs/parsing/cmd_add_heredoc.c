/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_inoutfile_ihoa(t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	var_cmd->size_file_tab = len_tab_char(cmd[var_cmd->index_cmd].inoutfile);
	var_cmd->new_tab_char = add_double_tab_char
		(cmd[var_cmd->index_cmd].inoutfile,
			var_cmd->file_temp, var_cmd->size_file_tab);
	if (!var_cmd->new_tab_char)
		return (free(var_cmd->file_temp), var_cmd->file_temp = NULL, -1);
	cmd[var_cmd->index_cmd].inoutfile = var_cmd->new_tab_char;
	var_cmd->new_tab_int = add_double_tab_int(cmd[var_cmd->index_cmd].ihoa, 4,
			var_cmd->size_file_tab);
	if (!var_cmd->new_tab_int)
		return (-1);
	cmd[var_cmd->index_cmd].ihoa = var_cmd->new_tab_int;
	return (0);
}

int	appliquer_add_cmd_heredoc_1(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !token->str || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	var_cmd->file_temp = ft_strdup("/dev/null");
	if (!var_cmd->file_temp)
		return (-1);
	if (appliquer_inoutfile_ihoa(cmd, var_cmd) == -1)
		return (-1);
	var_cmd->n = cmd[var_cmd->index_cmd].compter_heredoc;
	var_cmd->limiter_env = check_heredoc_env(token->str);
	if (check_quote_limiter(token->str))
		var_cmd->limiter_sans_quote = enlever_quote_dans_token(token->str);
	else
		var_cmd->limiter_sans_quote = ft_strdup(token->str);
	if (!var_cmd->limiter_sans_quote)
		return (-1);
	return (0);
}

int	appliquer_add_cmd_heredoc_2(t_cmd *cmd, t_var_cmd *var_cmd)
{
	var_cmd->new_tab_char = add_double_tab_char(cmd[var_cmd->index_cmd].limiter,
			var_cmd->limiter_sans_quote, var_cmd->n);
	if (!var_cmd->new_tab_char)
		return (free(var_cmd->limiter_sans_quote),
			var_cmd->limiter_sans_quote = NULL, -1);
	cmd[var_cmd->index_cmd].limiter = var_cmd->new_tab_char;
	var_cmd->new_tab_int = add_double_tab_int(cmd[var_cmd->index_cmd].hd_env,
			var_cmd->limiter_env, var_cmd->n);
	if (!var_cmd->new_tab_int)
		return (-1);
	cmd[var_cmd->index_cmd].hd_env = var_cmd->new_tab_int;
	cmd[var_cmd->index_cmd].compter_heredoc++;
	cmd[var_cmd->index_cmd].heredoc = 1;
	cmd[var_cmd->index_cmd].compter_ihoa++;
	return (0);
}

int	appliquer_add_cmd_heredoc_all(t_token *token, t_cmd *cmd,
		t_var_cmd *var_cmd)
{
	if (!token || !token->str || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	if (appliquer_add_cmd_heredoc_1(token, cmd, var_cmd) == -1)
		return (-1);
	if (appliquer_add_cmd_heredoc_2(cmd, var_cmd) == -1)
		return (-1);
	return (0);
}

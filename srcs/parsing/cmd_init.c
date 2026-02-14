/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd(t_cmd *cmd, int j)
{
	cmd[j].cmd = NULL;
	cmd[j].inoutfile = NULL;
	cmd[j].temp_heredoc = NULL;
	cmd[j].limiter = NULL;
	cmd[j].compter_heredoc = 0;
	cmd[j].in_heredoc = NULL;
	cmd[j].compter_ihoa = 0;
	cmd[j].hd_env = NULL;
	cmd[j].ihoa = NULL;
	cmd[j].heredoc = 0;
	cmd[j].pid_heredoc = -1;
	cmd[j].fd_in = -1;
	cmd[j].fd_out = -1;
	cmd[j].inout_fail = 0;
}

t_cmd	*malloc_cmd(t_token *token)
{
	int		compter_pipe;
	int		nbr_cmd;
	int		j;
	t_cmd	*cmd;

	j = 0;
	compter_pipe = count_pipe(token);
	nbr_cmd = compter_pipe + 1;
	cmd = malloc(sizeof(t_cmd) * nbr_cmd);
	if (!cmd)
		return (NULL);
	while (j < nbr_cmd)
	{
		init_cmd(cmd, j);
		j++;
	}
	return (cmd);
}

void	init_var_cmd(t_var_cmd *var_cmd, int *resultat)
{
	if (!var_cmd)
		return ;
	*resultat = 0;
	var_cmd->index_cmd = 0;
	var_cmd->i = 0;
	var_cmd->n = 0;
	var_cmd->redir_existe = 0;
	var_cmd->mot_temp = NULL;
	var_cmd->file_temp = NULL;
	var_cmd->size_file_tab = 0;
	var_cmd->limiter_sans_quote = 0;
	var_cmd->limiter_env = 0;
	var_cmd->new_tab_char = NULL;
	var_cmd->new_tab_int = NULL;
	var_cmd->size_cmd = 0;
}

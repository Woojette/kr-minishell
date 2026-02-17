/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:42:42 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/17 02:43:05 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	no_path(t_mini *mini, char *cmd)
{
	int	len;

	len = ft_strlen(cmd);
	write(2, cmd, len);
	write(2, ": No such file or directory\n", 28);
	child_exit_nb(mini, 127);
}

void	invalid_cmd(t_mini *mini, char *not_cmd)
{
	int	len;

	len = ft_strlen(not_cmd);
	mini->exit_status = 127;
	write(2, not_cmd, len);
	write(2, ": command not found\n", 21);
	child_exit(mini);
}

void	ft_execute(t_mini *mini, t_cmd *cmd)
{
	char	*valid_cmd;

	if (cmd->inout_fail)
		child_exit_nb(mini, 1);
	if (!cmd || !cmd->cmd || !cmd->cmd[0])
		child_exit_nb(mini, 0);
	valid_cmd = cmd_path_center(mini, cmd->cmd[0]);
	if (!valid_cmd)
	{
		if (mini->exit_status == 126)
			child_exit_nb(mini, 126);
		invalid_cmd(mini, cmd->cmd[0]);
	}
	execve(valid_cmd, cmd->cmd, mini->env);
	free(valid_cmd);
	perror(cmd->cmd[0]);
	if (errno == ENOENT)
		child_exit_nb(mini, 127);
}

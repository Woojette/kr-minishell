/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager_util_child.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:27:52 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 17:16:24 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	before(t_mini *mini, t_cmd cmd)
{
	if (!cmd.cmd || !cmd.cmd[0])
		child_exit(mini);
}

void	first(t_mini *mini, int *pipe_fd)
{
	ft_close(pipe_fd[0]);
	pipe_fd[0] = -1;
	c_dup2(mini, pipe_fd[1], 1);
	ft_close(pipe_fd[1]);
	pipe_fd[1] = -1;
}

void	middle(t_mini *mini, int *pipe_fd)
{
	c_dup2(mini, mini->pipe_read_end, 0);
	ft_close(mini->pipe_read_end);
	pipe_fd[0] = -1;
	c_dup2(mini, pipe_fd[1], 1);
	ft_close(pipe_fd[1]);
	pipe_fd[1] = -1;
	ft_close(pipe_fd[0]);
	pipe_fd[0] = -1;
}

void	last(t_mini *mini)
{
	c_dup2(mini, mini->pipe_read_end, 0);
	ft_close(mini->pipe_read_end);
	mini->pipe_read_end = -1;
}

void	set_pipe_exit(t_mini *mini, int *pipe_fd)
{
	if (pipe_fd[0] >= 0)
		ft_close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		ft_close(pipe_fd[1]);
	if (mini->pipe_read_end >= 0)
		ft_close(mini->pipe_read_end);
}

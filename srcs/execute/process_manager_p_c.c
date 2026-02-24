/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager_p_c.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:29:24 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/24 01:08:03 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cc_cut(t_mini *mini, int *pipe_fd, int i)
{
	if (i == 0)
		first(mini, pipe_fd);
	else if (i != 0 && i != mini->nbr_cmd - 1)
		middle(mini, pipe_fd);
	else
		last(mini);
}

static void	cc_cut2(t_mini *mini, int *pipe_fd, int i)
{
	int	save_es;

	save_es = mini->exit_status;
	ft_exit2(mini->cmd_array[i].cmd, mini);
	set_pipe_exit(mini, pipe_fd);
	if (mini->exit_pl_arg == 1)
	{
		mini->exit_pl_arg = -1;
		child_exit_nb(mini, 1);
	}
	child_exit_nb(mini, save_es);
}

static void	cc_cut3(t_mini *mini, int *pipe_fd, int i, int type)
{
	int	save_es;

	save_es = mini->exit_status;
	execute_built_in2(mini, mini->cmd_array[i].cmd, type);
	set_pipe_exit(mini, pipe_fd);
	child_exit_nb(mini, save_es);
}

void	child_center(t_mini *mini, t_cmd cmd, int *pipe_fd, int i)
{
	int	type;

	before(mini, cmd);
	type = is_built_in(mini->cmd_array[i].cmd[0]);
	if (mini->nbr_cmd > 1)
		cc_cut(mini, pipe_fd, i);
	apply_redirection_child(mini, &cmd);
	if (type == T_NOT_BUILT_IN)
	{
		ft_execute(mini, &mini->cmd_array[i]);
		if (mini->exit_status == 0)
		{
			set_pipe_exit(mini, pipe_fd);
			child_exit_nb(mini, 127);
		}
	}
	else
	{
		if (type == T_EXIT)
			cc_cut2(mini, pipe_fd, i);
		cc_cut3(mini, pipe_fd, i, type);
	}
}

void	parent_center(t_mini *mini, int pipe_fd[2], int i)
{
	if (!mini->cmd_array)
		return ;
	if (i < mini->nbr_cmd - 1)
	{
		if (mini->pipe_read_end != -1)
		{
			ft_close(mini->pipe_read_end);
			mini->pipe_read_end = pipe_fd[0];
		}
		else
		{
			mini->pipe_read_end = pipe_fd[0];
		}
		ft_close(pipe_fd[1]);
	}
	else
	{
		if (mini->pipe_read_end != -1)
			ft_close(mini->pipe_read_end);
	}
}

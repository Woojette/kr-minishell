/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager_p_c.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:29:24 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/15 18:21:45 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_center(t_mini *mini, t_cmd cmd, int *pipe_fd, int i)
{
	const int	type = is_built_in(mini->cmd_array[i].cmd[0]);

	before(mini, cmd);
	if (mini->nbr_cmd > 1)
	{
		if (i == 0)
			first(mini, pipe_fd);
		else if (i != 0 && i != mini->nbr_cmd - 1)
			middle(mini, pipe_fd);
		else
			last(mini);
	}
	apply_redirection_child(mini, &cmd);
	if (type == T_NOT_BUILT_IN)
	{
		ft_execute(mini, &mini->cmd_array[i]);
		if (mini->exit_status == 0)
			child_exit_nb(mini, 127);
	}
	else
	{
		execute_built_in2(mini, mini->cmd_array[i].cmd, type);
		child_exit(mini);
	}
	set_pipe_exit(mini, pipe_fd);
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

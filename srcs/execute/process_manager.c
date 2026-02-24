/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:47:01 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/24 01:39:23 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	status_exit(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	wait_pid(t_mini *mini, pid_t *pids)
{
	int		i;
	int		status;
	pid_t	last_pid;

	last_pid = -1;
	i = 0;
	while (i < mini->nbr_cmd)
	{
		last_pid = waitpid(pids[i], &status, 0);
		if (last_pid == -1)
			break ;
		if (last_pid == pids[mini->nbr_cmd - 1])
		{
			print_sig_msg(status);
			mini->exit_status = status_exit(status);
		}
		i++;
	}
}

static void	while_cut_util(t_mini *mini, int *pipe_fd, int save_es, int i)
{
	set_signal_exec_child();
	child_center(mini, mini->cmd_array[i], pipe_fd, i);
	save_es = mini->exit_status;
	child_exit_nb(mini, save_es);
}

static void	while_cut(t_mini *mini, pid_t *pids)
{
	int		i;
	int		pipe_fd[2];
	pid_t	child_id;
	int		save_es;

	i = 0;
	while (i < mini->nbr_cmd)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (i < mini->nbr_cmd - 1)
			if (pipe(pipe_fd) == -1)
				fatal_error(mini, "pipe_error");
		child_id = fork();
		if (child_id < 0)
			fatal_error(mini, "fork_error");
		if (child_id == 0)
		{
			save_es = mini->exit_status;
			while_cut_util(mini, pipe_fd, save_es, i);
		}
		pids[i] = child_id;
		parent_center(mini, pipe_fd, i);
		i++;
	}
}

void	fork_center(t_mini *mini)
{
	pid_t	pids[MAX_CMDS];

	if (mini->nbr_cmd > MAX_CMDS)
		fatal_error(mini, "cmds more than 1024");
	set_signal_parent_wait();
	mini->pipe_read_end = -1;
	while_cut(mini, pids);
	wait_pid(mini, pids);
	init_signaux();
}

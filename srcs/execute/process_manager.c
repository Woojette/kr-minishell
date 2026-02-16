/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:47:01 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 01:16:28 by yookyeoc         ###   ########.fr       */
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
		if (last_pid == pids[mini->nbr_cmd - 1])
			mini->exit_status = status_exit(status);
		i++;
	}
}

static void	while_cut(t_mini *mini, pid_t *pids)
{
	int		i;
	int		pipe_fd[2];
	pid_t	child_id;

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
			set_signal_exec_child();
			child_center(mini, mini->cmd_array[i], pipe_fd, i);
		}
		pids[i] = child_id;
		parent_center(mini, pipe_fd, i);
		i++;
	}
}

void	fork_center(t_mini *mini)
{
	pid_t	*pids;

	set_signal_parent_wait();
	redirection_center(mini);
	pids = malloc(sizeof(pid_t) * mini->nbr_cmd);
	if (!pids)
		fatal_error(mini, "pids_malloc_error");
	mini->pipe_read_end = -1;
	while_cut(mini, pids);
	wait_pid(mini, pids);
	init_signaux();
	free(pids);
}

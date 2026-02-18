/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_refac.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:17:10 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/18 23:19:49 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reduce_1(t_mini *mini, int j)
{
	mini->cmd_array[j].inout_fail = 1;
	mini->exit_status = 1;
	free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
	mini->cmd_array[j].temp_heredoc = NULL;
	init_signaux();
}

void	reduce_2(t_mini *mini, int j, int status)
{
	int	exit_signal;

	exit_signal = WTERMSIG(status);
	mini->cmd_array[j].inout_fail = 1;
	mini->exit_status = 128 + exit_signal;
	free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
	mini->cmd_array[j].temp_heredoc = NULL;
	init_signaux();
}

int	cut_wa(t_mini *mini, int j, int exit_status)
{
	mini->cmd_array[j].inout_fail = 1;
	mini->exit_status = exit_status;
	free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
	mini->cmd_array[j].temp_heredoc = NULL;
	init_signaux();
	return (-1);
}

int	work_appliquer(t_mini *mini, int j, int n, t_hd_enfant *he)
{
	int	exit_status;
	int	status;

	if (preparer_temp_file_name(mini, j, n) == -1)
		return (reduce_1(mini, j), -1);
	mini->cmd_array[j].pid_heredoc = fork();
	if (mini->cmd_array[j].pid_heredoc == -1)
		return (reduce_1(mini, j), -1);
	if (mini->cmd_array[j].pid_heredoc == 0)
		appliquer_heredoc_enfant(he);
	if (waitpid(mini->cmd_array[j].pid_heredoc, &status, 0) == -1)
		return (reduce_1(mini, j), -1);
	if (WIFSIGNALED(status))
		return (reduce_2(mini, j, status), -1);
	else if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
			return (cut_wa(mini, j, exit_status));
	}
	else
		return (reduce_1(mini, j), -1);
	if (!mini->cmd_array[j].temp_heredoc || !mini->cmd_array[j].temp_heredoc[n])
		return (reduce_1(mini, j), -1);
	return (0);
}

int	appliquer_heredoc_cmd(t_mini *mini, int j)
{
	int			n;
	int			set;
	t_hd_enfant	he;

	set = before_appliquer(mini, j);
	if (set <= 0)
		return (set);
	n = 0;
	while (n < mini->cmd_array[j].compter_heredoc)
	{
		if (work_appliquer(mini, j, n, &he) == -1)
			return (-1);
		n++;
	}
	init_signaux();
	return (0);
}

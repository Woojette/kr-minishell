/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_enfant_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:50:57 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/18 17:50:58 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	util_close_exit(t_mini *mini, int fd_temp, int exit_flag)
{
	hd_child_cleanup(mini, fd_temp);
	exit(exit_flag);
}

void	sig_hd(t_mini *mini)
{
	(void)mini;
	signal(SIGINT, sig_hd_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_hd_handler(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	perexit(t_mini *mini, int exit_flag)
{
	perror("open temp");
	hd_child_cleanup(mini, -1);
	exit(exit_flag);
}

void	util_struct_check(t_mini *mini, int j, int n)
{
	if (!mini || j < 0 || j >= mini->nbr_cmd)
	{
		hd_child_cleanup(mini, -1);
		exit(1);
	}
	if (n < 0 || n >= mini->cmd_array[j].compter_heredoc)
	{
		hd_child_cleanup(mini, -1);
		exit(1);
	}
	if (!mini->cmd_array[j].temp_heredoc || !mini->cmd_array[j].temp_heredoc[n])
	{
		hd_child_cleanup(mini, -1);
		exit(1);
	}
}

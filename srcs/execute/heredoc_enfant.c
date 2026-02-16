/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_enfant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 12:53:25 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 01:15:02 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	util_struct_check(t_mini *mini, int j, int n)
{
	if (!mini || j < 0 || j >= mini->nbr_cmd)
		child_exit_nb(mini, 1);
	if (n < 0 || n >= mini->cmd_array[j].compter_heredoc)
		child_exit_nb(mini, 1);
	if (!mini->cmd_array[j].temp_heredoc || !mini->cmd_array[j].temp_heredoc[n])
		child_exit_nb(mini, 1);
}

void	util_close_exit(t_mini *mini, int fd_temp, int exit_flag)
{
	(void)mini;
	ft_close(fd_temp);
	exit(exit_flag);
}

void	sig_hd(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	perexit(t_mini *mini, int exit_flag)
{
	perror("open temp");
	child_exit_nb(mini, exit_flag);
}

void	appliquer_heredoc_enfant(t_mini *mini, int j, int n)
{
	int	resultat;
	int	fd_temp;

	sig_hd();
	util_struct_check(mini, j, n);
	fd_temp = open(mini->cmd_array[j].temp_heredoc[n],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_temp == -1)
		perexit(mini, 1);
	if (!mini->cmd_array[j].limiter || !mini->cmd_array[j].limiter[n])
		util_close_exit(mini, fd_temp, 1);
	resultat = collecter_heredoc_lines(fd_temp, mini, j, n);
	if (resultat == 1)
	{
		print_heredoc_warning_ctrl_d(mini->cmd_array[j].limiter[n]);
		util_close_exit(mini, fd_temp, 0);
	}
	if (resultat < 0)
	{
		perror("collecter heredoc");
		ft_close(fd_temp);
		unlink(mini->cmd_array[j].temp_heredoc[n]);
		child_exit_nb(mini, 1);
	}
	util_close_exit(mini, fd_temp, 0);
}

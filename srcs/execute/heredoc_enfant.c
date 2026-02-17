/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_enfant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 12:53:25 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/17 04:29:22 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	hd_child_cleanup(t_mini *mini, int fd_temp)
{
	t_m	*m;
	int	i;

	if (fd_temp >= 0)
		ft_close(fd_temp);
	m = (t_m *)mini->m_ptr;
	if (m)
	{
		if (m->parsing)
			free_tokens(&m->parsing);
		m->parsing = NULL;
		if (m->line)
			free(m->line);
		m->line = NULL;
	}
	i = 0;
	while (i < mini->nbr_cmd)
	{
		if (mini->cmd_array[i].temp_heredoc)
		{
			free_tab_char(mini->cmd_array[i].temp_heredoc);
			mini->cmd_array[i].temp_heredoc = NULL;
		}
		i++;
	}
	rl_clear_history();
	free_mini(mini);
}

static void	sig_hd_handler(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
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

void	perexit(t_mini *mini, int exit_flag)
{
	perror("open temp");
	hd_child_cleanup(mini, -1);
	exit(exit_flag);
}

void	appliquer_heredoc_enfant(t_mini *mini, int j, int n)
{
	int	resultat;
	int	fd_temp;

	sig_hd(mini);
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
	if (resultat == -2)
	{
		ft_close(fd_temp);
		unlink(mini->cmd_array[j].temp_heredoc[n]);
		hd_child_cleanup(mini, -1);
		exit(130);
	}
	if (resultat < 0)
	{
		perror("collecter heredoc");
		ft_close(fd_temp);
		unlink(mini->cmd_array[j].temp_heredoc[n]);
		hd_child_cleanup(mini, -1);
		exit(1);
	}
	util_close_exit(mini, fd_temp, 0);
}

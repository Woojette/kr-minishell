/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_enfant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 12:53:25 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/18 21:19:41 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cut_hcc(t_mini *mini)
{
	rl_clear_history();
	free_mini(mini);
}

void	hd_child_cleanup(t_mini *mini, int fd_temp)
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
	i = -1;
	while (++i < mini->nbr_cmd)
	{
		if (mini->cmd_array[i].temp_heredoc)
		{
			free_tab_char(mini->cmd_array[i].temp_heredoc);
			mini->cmd_array[i].temp_heredoc = NULL;
		}
	}
	cut_hcc(mini);
}

static void	cut_ape(t_mini *mini, int fd_temp, int j, int n)
{
	ft_close(fd_temp);
	unlink(mini->cmd_array[j].temp_heredoc[n]);
	hd_child_cleanup(mini, -1);
}

static void	warningcd_close_exit(t_mini *mini, int fd_temp, int j, int n)
{
	print_heredoc_warning_ctrl_d(mini->cmd_array[j].limiter[n]);
	util_close_exit(mini, fd_temp, 0);
}

void	appliquer_heredoc_enfant(t_hd_enfant *he)
{
	sig_hd(he->mini);
	util_struct_check(he->mini, he->j, he->n);
	he->fd_temp = open(he->mini->cmd_array[he->j].temp_heredoc[he->n],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (he->fd_temp == -1)
		perexit(he->mini, 1);
	if (!he->mini->cmd_array[he->j].limiter
		|| !he->mini->cmd_array[he->j].limiter[he->n])
		util_close_exit(he->mini, he->fd_temp, 1);
	he->resultat = collecter_heredoc_lines(he->fd_temp, he->mini, he->j, he->n);
	if (he->resultat == 1)
		warningcd_close_exit(he->mini, he->fd_temp, he->j, he->n);
	if (he->resultat == -2)
	{
		cut_ape(he->mini, he->fd_temp, he->j, he->n);
		exit(130);
	}
	if (he->resultat < 0)
	{
		perror("collecter heredoc");
		cut_ape(he->mini, he->fd_temp, he->j, he->n);
		exit(1);
	}
	util_close_exit(he->mini, he->fd_temp, 0);
}

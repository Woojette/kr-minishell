/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_inout_refac.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 22:23:40 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 17:16:46 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir(t_mini *mini, int i, t_redir *redir)
{
	int	fd;

	redir->path = NULL;
	if (redir->type == HEREDOC)
	{
		if (!mini->cmd_array[i].temp_heredoc
			|| !mini->cmd_array[i].temp_heredoc[redir->k])
			return (-1);
		redir->path = mini->cmd_array[i].temp_heredoc[redir->k];
		fd = open(redir->path, O_RDONLY);
		if (fd >= 0)
			redir->k++;
		return (fd);
	}
	redir->path = mini->cmd_array[i].inoutfile[redir->n];
	if (redir->type == INF)
		return (open(redir->path, O_RDONLY));
	if (redir->type == TRUNC)
		return (open(redir->path, O_WRONLY | O_TRUNC | O_CREAT, 0644));
	if (redir->type == APPEND)
		return (open(redir->path, O_WRONLY | O_APPEND | O_CREAT, 0644));
	return (-1);
}

void	remplacer_fd(int *dst, int fd)
{
	if (*dst != -1)
		ft_close(*dst);
	*dst = fd;
}

int	inout_redir(t_mini *mini, int i)
{
	t_redir	redir;
	int		fd;
	int		inhd;

	if (before_inout(mini, i) < 0)
		return (-1);
	redir.n = 0;
	redir.k = 0;
	while (mini->cmd_array[i].inoutfile[redir.n])
	{
		redir.type = get_redir_type(mini, i, redir.n);
		fd = open_redir(mini, i, &redir);
		inhd = (redir.type == INF || redir.type == HEREDOC);
		if (fd < 0)
		{
			if (mini->cmd_array[i].fd_in >= 0)
			{
				close(mini->cmd_array[i].fd_in);
				mini->cmd_array[i].fd_in = -1;
			}
			if (mini->cmd_array[i].fd_out >= 0)
			{
				close(mini->cmd_array[i].fd_out);
				mini->cmd_array[i].fd_out = -1;
			}
			return (fail_redir(mini, i, &redir, inhd));
		}
		if (inhd)
			remplacer_fd(&mini->cmd_array[i].fd_in, fd);
		else
			remplacer_fd(&mini->cmd_array[i].fd_out, fd);
		if (!mini->cmd_array[i].inout_fail)
			mini->exit_status = 0;
		redir.n++;
	}
	return (0);
}

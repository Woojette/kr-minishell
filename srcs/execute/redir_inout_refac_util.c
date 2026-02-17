/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_inout_refac_util.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 01:28:02 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 18:14:26 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_restore(t_mini *mini, int i, int k)
{
	if (mini->cmd_array[i].temp_heredoc && mini->cmd_array[i].temp_heredoc[k])
	{
		unlink(mini->cmd_array[i].temp_heredoc[k]);
		free(mini->cmd_array[i].temp_heredoc[k]);
		mini->cmd_array[i].temp_heredoc[k] = NULL;
	}
	init_signaux();
}

int	get_redir_type(t_mini *mini, int i, int n)
{
	if (!mini->cmd_array[i].ihoa)
		return (DEFAULT);
	return (mini->cmd_array[i].ihoa[n]);
}

int	fail_redir(t_mini *mini, int i, t_redir *redir, int inhd)
{
	if (redir->type == HEREDOC)
		heredoc_restore(mini, i, redir->k);
	open_fail(mini, i, redir->path, inhd);
	return (-1);
}

void	open_fail(t_mini *mini, int i, const char *p, int inhd)
{
	if (p)
		perror(p);
	mini->cmd_array[i].inout_fail = 1;
	if (mini->nbr_cmd == 1)
		mini->exit_status = 1;
	if (inhd && mini->cmd_array[i].fd_in != -1)
	{
		ft_close(mini->cmd_array[i].fd_in);
		mini->cmd_array[i].fd_in = -1;
	}
	if (!inhd && mini->cmd_array[i].fd_out != -1)
	{
		ft_close(mini->cmd_array[i].fd_out);
		mini->cmd_array[i].fd_out = -1;
	}
}

int	before_inout(t_mini *mini, int i)
{
	if (!mini || i < 0 || i >= mini->nbr_cmd)
		return (-1);
	if (!mini->cmd_array || !mini->cmd_array[i].inoutfile)
		return (-1);
	if (mini->cmd_array[i].inout_fail)
		return (-1);
	return (0);
}

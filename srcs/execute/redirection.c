/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:47:45 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 01:30:51 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirection_center(t_mini *mini)
{
	int		i;
	t_cmd	*c;

	i = 0;
	if (!mini || !mini->cmd_array || mini->nbr_cmd <= 0)
		return (-1);
	while (i < mini->nbr_cmd)
	{
		c = &mini->cmd_array[i];
		if (!c->inout_fail)
			inout_redir(mini, i);
		i++;
	}
	return (0);
}

void	apply_redirection_child(t_mini *mini, t_cmd *cmd)
{
	if (cmd->inout_fail)
	{
		if (mini->exit_status == 0)
			child_exit_nb(mini, mini->exit_status = 1);
	}
	if (cmd->fd_in >= 0)
	{
		c_dup2(mini, cmd->fd_in, 0);
		ft_close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out >= 0)
	{
		c_dup2(mini, cmd->fd_out, 1);
		ft_close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}

void	obar_util(t_mini *mini, int flag)
{
	t_cmd	*c;

	c = &mini->cmd_array[0];
	(void)mini;
	if (flag == 0)
	{
		p_dup2(mini, c->fd_in, 0);
		ft_close(c->fd_in);
		c->fd_in = -1;
	}
	if (flag == 1)
	{
		p_dup2(mini, c->fd_out, 1);
		ft_close(c->fd_out);
		c->fd_out = -1;
	}
}

int	one_builtin_avec_redirs(t_mini *mini)
{
	t_cmd		*c;
	int			type;
	const int	in_save = dup(0);
	const int	out_save = dup(1);

	c = &mini->cmd_array[0];
	type = is_built_in(c->cmd[0]);
	if (c->inout_fail)
		return (mini->exit_status);
	if (!c->cmd || !c->cmd[0])
		return (mini->exit_status = 1);
	if (c->fd_in >= 0)
	{
		obar_util(mini, 0);
	}
	if (c->fd_out >= 0)
	{
		obar_util(mini, 1);
	}
	execute_built_in(mini, c->cmd, type);
	p_dup2(mini, in_save, 0);
	p_dup2(mini, out_save, 1);
	ft_close(in_save);
	ft_close(out_save);
	return (mini->exit_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 06:09:37 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/24 01:07:00 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_save(int in_save, int out_save)
{
	close(in_save);
	close(out_save);
}

int	close_save_exit_status(t_mini *mini, int in_save, int out_save)
{
	close_save(in_save, out_save);
	return (mini->exit_status);
}

int	obar_t_exit(t_mini *mini, t_cmd *c, int in_save, int out_save)
{
	ft_close_save(in_save, out_save);
	ft_exit(c->cmd, mini);
	if (mini->exit_pl_arg == 1)
		return (mini->exit_pl_arg = -1);
	set_pipe(mini, in_save, out_save);
	return (mini->exit_status);
}

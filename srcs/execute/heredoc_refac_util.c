/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_refac_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:29:07 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/18 17:29:08 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	before_appliquer(t_mini *mini, int j)
{
	t_cmd	*cmd;

	if (!mini || j < 0 || j >= mini->nbr_cmd)
		return (-1);
	if (mini->cmd_array[j].compter_heredoc <= 0)
		return (0);
	cmd = &mini->cmd_array[j];
	set_signal_parent_wait();
	if (mini->cmd_array[j].temp_heredoc == NULL)
	{
		cmd->temp_heredoc = ft_calloc(cmd->compter_heredoc + 1, sizeof(char *));
		if (!mini->cmd_array[j].temp_heredoc)
		{
			mini->cmd_array[j].inout_fail = 1;
			mini->exit_status = 1;
			init_signaux();
			return (-1);
		}
	}
	return (1);
}

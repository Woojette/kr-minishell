/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_center.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:33:18 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/18 17:33:19 by yookyeoc         ###   ########.fr       */
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

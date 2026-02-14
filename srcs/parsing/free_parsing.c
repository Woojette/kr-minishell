/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_temp_heredoc(char **temp)
{
	int	i;

	if (!temp)
		return ;
	i = 0;
	while (temp[i])
	{
		unlink(temp[i]);
		free(temp[i]);
		i++;
	}
	free(temp);
}

void	free_mini(t_mini *mini)
{
	if (!mini)
		return ;
	if (mini->cmd_array)
		free_cmd_all(mini->cmd_array, mini->nbr_cmd);
	free_tab_char(mini->path_array);
	mini->path_array = NULL;
	free(mini);
}

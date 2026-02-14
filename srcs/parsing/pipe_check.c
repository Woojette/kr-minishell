/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_fin(char *line)
{
	int	i;

	i = 0;
	while (line[i])
		i++;
	i--;
	while (i >= 0 && line[i] == ' ')
		i--;
	if (i >= 0 && line[i] == '|')
		return (1);
	return (0);
}

int	count_pipe(t_token *token)
{
	int		count;
	t_token	*temp;

	count = 0;
	temp = token;
	while (temp)
	{
		if (temp->type_token == T_PIPE)
			count++;
		temp = temp->next;
	}
	return (count);
}

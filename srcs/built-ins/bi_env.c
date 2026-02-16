/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_mini *mini)
{
	int	j;

	j = 0;
	while (mini->env[j] != NULL)
	{
		printf("%s\n", mini->env[j]);
		j++;
	}
}

int	ft_check_env_egal(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	if (str[i] == '\0')
		return (-1);
	return (i);
}

int	ft_check_env_double(char *str, t_mini *mini)
{
	int	i;
	int	j;
	int	egal;

	i = 0;
	j = 0;
	egal = ft_check_env_egal(str);
	if (egal == -1)
		return (-1);
	while (mini->env[j] != NULL)
	{
		if (ft_strncmp(str, mini->env[j], egal + 1) == 0)
			return (j);
		j++;
	}
	return (-1);
}

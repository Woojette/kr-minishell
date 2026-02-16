/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	same_key(char *str1, char *str2)
{
	if (key_len(str1) != key_len(str2))
		return (0);
	return (ft_strncmp(str1, str2, key_len(str1)) == 0);
}

int	key_index(char *str, char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (-1);
	while (tab[i])
	{
		if (same_key(str, tab[i]))
			return (i);
		i++;
	}
	return (-1);
}

int	has_samekey(char *str, t_mini mini)
{
	int	i;
	int	len;

	i = 0;
	if (equal_checker(str) == 0)
	{
		while (mini.save_ex[i])
		{
			if ((ft_strncmp(str, mini.save_ex[i], ft_strlen(str)) == 0))
				return (0);
			i++;
		}
	}
	i = 0;
	if (equal_checker(str) == 1)
	{
		while (mini.save_ex[i])
		{
			len = key_len(mini.save_ex[i]);
			if (ft_strncmp(str, mini.save_ex[i], len) == 0)
				return (1);
			i++;
		}
	}
	return (-1);
}

int	has_samekey2(char *str, t_mini *mini)
{
	int	i;
	int	len;

	i = 0;
	while (mini->save_ex[i])
	{
		len = key_len(mini->save_ex[i]);
		if (ft_strncmp(str, mini->save_ex[i], len) == 0)
			return (1);
		i++;
	}
	return (-1);
}

int	same_checker(char *str, t_mini mini)
{
	int	i;
	int	len;

	i = 0;
	while (mini.save_ex[i])
	{
		len = ft_strlen(mini.save_ex[i]);
		if (ft_strcmp(str, mini.save_ex[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

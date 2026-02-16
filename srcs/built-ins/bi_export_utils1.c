/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	first_checker(char c)
{
	if (ft_isalpha(c) != 0 || c == '_')
		return (1);
	return (0);
}

int	key_letters(char *str)
{
	int	i;

	i = 0;
	if (!str || !first_checker(str[i]))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (ft_isalnum(str[i]) || str[i] == '_')
			i++;
		else
			return (0);
	}
	return (1);
}

int	key_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
	}
	return (i);
}

int	equal_checker(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			count++;
			return (1);
		}
		i++;
	}
	if (count == 0)
		return (0);
	else if (count == 1)
		return (1);
	else
		return (-1);
}

int	has_valid_key(char *str)
{
	int check;
	check = equal_checker(str);
	if (check == 0 || check == 1)
	{
		if (key_letters(str) == 1)
			return (1);
	}
	return (0);
}

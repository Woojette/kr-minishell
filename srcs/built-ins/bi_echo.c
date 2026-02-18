/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/17 00:52:20 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo_option_n(char *str)
{
	int	i;

	if (!str)
		return (0);
	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] == 'n')
			i++;
		else
			return (0);
	}
	return (1);
}

void	ft_echo_all(t_mini *mini, char **tab)
{
	int	j;
	int	option_n;

	j = 1;
	option_n = 0;
	while (tab[j] != NULL && ft_echo_option_n(tab[j]) == 1)
		j++;
	if (j > 1)
		option_n = 1;
	while (tab[j] != NULL)
	{
		printf("%s", tab[j]);
		if (tab[j + 1] != NULL)
			printf(" ");
		j++;
	}
	if (option_n == 0)
		printf("\n");
	mini->exit_status = 0;
}

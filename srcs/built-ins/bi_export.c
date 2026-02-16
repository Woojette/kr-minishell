/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_print(t_mini *mini)
{
	int	i;
	int	j;
	int	flag;

	i = -1;
	while (mini->save_ex[++i])
	{
		write(1, "export ", 7);
		j = -1;
		flag = 0;
		while (mini->save_ex[i][++j])
		{
			write(1, &(mini->save_ex[i][j]), 1);
			if (mini->save_ex[i][j] == '=')
			{
				flag = 1;
				write(1, "\"", 1);
			}
		}
		if (flag == 1)
			write(1, "\"", 1);
		write(1, "\n", 1);
	}
}

int	ft_export_all(char **tab, t_mini *mini)
{
	int	i;

	i = 0;
	if (tab[i] && !tab[1])
	{
		export_sort(mini);
		export_print(mini);
		return (0);
	}
	i = 1;
	while (tab[i] != NULL)
	{
		save_export(tab[i], mini);
		i++;
	}
	return (0);
}

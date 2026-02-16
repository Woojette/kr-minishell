/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(char **tab)
{
	int	j;

	if (!tab)
		return ;
	j = 0;
	while (tab[j] != NULL)
		j++;
	while (j > 0)
	{
		j--;
		if (tab[j])
		{
			free(tab[j]);
			tab[j] = NULL;
		}
	}
	free(tab);
}

void	ft_free_all(t_mini **mini)
{
	if ((*mini)->env)
		ft_free_tab((*mini)->env);
	if ((*mini))
		free(*mini);
}

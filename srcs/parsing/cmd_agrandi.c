/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_agrandi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_double_tab_char(char **tab, char *str, int size)
{
	char	**new_tab;
	int		j;

	if ((!tab && size > 0) || !str || size < 0)
		return (NULL);
	new_tab = malloc(sizeof(char *) * (size + 2));
	if (!new_tab)
		return (NULL);
	j = 0;
	if (tab)
	{
		while (j < size)
		{
			new_tab[j] = tab[j];
			j++;
		}
	}
	new_tab[j] = str;
	new_tab[j + 1] = NULL;
	return (free(tab), new_tab);
}

int	*add_double_tab_int(int *tab, int val, int size)
{
	int	*new_tab;
	int	j;

	if (!tab && size > 0)
		return (NULL);
	if (size < 0)
		return (NULL);
	new_tab = malloc(sizeof(int) * (size + 1));
	if (!new_tab)
		return (NULL);
	j = 0;
	if (tab)
	{
		while (j < size)
		{
			new_tab[j] = tab[j];
			j++;
		}
	}
	new_tab[j] = val;
	free(tab);
	return (new_tab);
}

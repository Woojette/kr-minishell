/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/17 01:05:27 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset_all(char **tab, t_mini *mini)
{
	int		j;
	t_unset	u;

	j = 1;
	while (tab[j] != NULL)
	{
		if (ft_unset(tab[j], &u, mini) == -1)
			return (-1);
		if (ft_unset2(tab[j], &u, mini) == -1)
			return (-1);
		j++;
	}
	return (0);
}

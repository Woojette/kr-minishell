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

void	ft_unset_init_int_zero(int *j, int *supprime, int *taille)
{
	*j = 0;
	*supprime = 0;
	*taille = 0;
}

int	ft_unset2(char *str, t_mini *mini)
{
	int		j;
	int		supprime;
	int		taille;
	int		len_str;
	char	**env_supprime;

	ft_unset_init_int_zero(&j, &supprime, &taille);
	while ((mini->save_ex)[taille] != NULL)
		taille++;
	len_str = ft_strlen(str);
	env_supprime = malloc(sizeof(char *) * (taille + 1));
	if (!env_supprime)
		return (-1);
	if (ft_strcmp(str, "PATH") == 0)
	{
		ft_free_tab(mini->path_array);
		mini->path_array = NULL;
	}
	while ((mini->save_ex)[j + supprime] != NULL)
	{
		if (((j + supprime) < taille) && ft_strncmp(str, (mini->save_ex)[j
				+ supprime], len_str) == 0 && ((mini->save_ex)[j
				+ supprime][len_str] == '=' || (mini->save_ex)[j
				+ supprime][len_str] == '\0'))
			supprime++;
		if ((j + supprime) < taille)
		{
			env_supprime[j] = ft_strdup((mini->save_ex)[j + supprime]);
			if (!env_supprime[j])
				return (ft_free_tab(env_supprime), -1);
		}
		j++;
	}
	env_supprime[j] = NULL;
	ft_free_tab((mini->save_ex));
	(mini->save_ex) = env_supprime;
	return (0);
}

int	ft_unset(char *str, t_mini *mini)
{
	int		j;
	int		supprime;
	int		taille;
	int		len_str;
	char	**env_supprime;

	ft_unset_init_int_zero(&j, &supprime, &taille);
	while ((mini->env)[taille] != NULL)
		taille++;
	len_str = ft_strlen(str);
	env_supprime = malloc(sizeof(char *) * (taille + 1));
	if (!env_supprime)
		return (-1);
	if (ft_strcmp(str, "PATH") == 0)
	{
		ft_free_tab(mini->path_array);
		mini->path_array = NULL;
	}
	while ((mini->env)[j + supprime] != NULL)
	{
		if (((j + supprime) < taille) && ft_strncmp(str, (mini->env)[j
				+ supprime], len_str) == 0 && (mini->env)[j
			+ supprime][len_str] == '=')
			supprime++;
		if ((j + supprime) < taille)
		{
			env_supprime[j] = ft_strdup((mini->env)[j + supprime]);
			if (!env_supprime[j])
				return (ft_free_tab(env_supprime), -1);
		}
		j++;
	}
	env_supprime[j] = NULL;
	ft_free_tab((mini->env));
	(mini->env) = env_supprime;
	return (0);
}

int	ft_unset_all(char **tab, t_mini *mini)
{
	int	j;

	j = 1;
	while (tab[j] != NULL)
	{
		if (ft_unset(tab[j], mini) == -1)
			return (-1);
		if (ft_unset2(tab[j], mini) == -1)
			return (-1);
		j++;
	}
	return (0);
}

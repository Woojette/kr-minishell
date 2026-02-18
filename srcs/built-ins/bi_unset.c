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

// void	ft_unset_init_int_zero(int *j, int *supprime, int *taille)
// {
// 	*j = 0;
// 	*supprime = 0;
// 	*taille = 0;
// }

int	ft_unset_init(char *str, t_unset *u, t_mini *mini)
{
	if (!str || !u || !mini || !mini->env)
		return (-1);
	u->j = 0;
	u->supprime = 0;
	u->taille = 0;
	while ((mini->env)[u->taille] != NULL)
		u->taille++;
	u->len_str = ft_strlen(str);
	u->env_supprime = ft_calloc((u->taille + 1), sizeof(char *));
	if (!u->env_supprime)
		return (-1);
	return (0);
}

int	ft_unset_path(char *str, t_mini *mini)
{
	if (!str || !mini)
		return (-1);
	if (ft_strcmp(str, "PATH") == 0)
	{
		ft_free_tab(mini->path_array);
		mini->path_array = NULL;
	}
	return (0);
}

int	ft_unset(char *str, t_unset *u, t_mini *mini)
{
	if (!str || !u || !mini || !mini->env)
		return (-1);
	if (ft_unset_init(str, u, mini) == -1)
		return (-1);
	if (ft_unset_path(str, mini) == -1)
		return (-1);
	while ((mini->env)[(u->j) + (u->supprime)] != NULL)
	{
		if ((((u->j) + (u->supprime)) < (u->taille))
			&& ft_strncmp(str, (mini->env)[(u->j) + (u->supprime)], u->len_str) == 0
			&& (mini->env)[(u->j) + (u->supprime)][u->len_str] == '=')
		{
			(u->supprime)++;
			continue ;
		}
		u->env_supprime[(u->j)] = ft_strdup((mini->env)[(u->j) + (u->supprime)]);
		if (!u->env_supprime[(u->j)])
			return (ft_free_tab(u->env_supprime), -1);
		u->j++;
	}
	u->env_supprime[u->j] = NULL;
	ft_free_tab((mini->env));
	return ((mini->env) = u->env_supprime, 0);
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

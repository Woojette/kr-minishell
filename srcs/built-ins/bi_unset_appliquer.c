/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset_appliquer.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_unset2_init(char *str, t_unset *u, t_mini *mini)
{
	if (!str || !u || !mini || !mini->save_ex)
		return (-1);
	u->j = 0;
	u->supprime = 0;
	u->taille = 0;
	while ((mini->save_ex)[u->taille] != NULL)
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
		return (ft_free_tab(u->env_supprime), -1);
	while ((mini->env)[(u->j) + (u->supprime)] != NULL)
	{
		if ((((u->j) + (u->supprime)) < (u->taille)) && ft_strncmp(str,
				(mini->env)[(u->j) + (u->supprime)], u->len_str) == 0
			&& (mini->env)[(u->j) + (u->supprime)][u->len_str] == '=')
		{
			(u->supprime)++;
			continue ;
		}
		u->env_supprime[(u->j)] = ft_strdup((mini->env)[(u->j)
				+ (u->supprime)]);
		if (!u->env_supprime[(u->j)])
			return (ft_free_tab(u->env_supprime), -1);
		u->j++;
	}
	u->env_supprime[u->j] = NULL;
	ft_free_tab((mini->env));
	return ((mini->env) = u->env_supprime, 0);
}

int	ft_unset2(char *str, t_unset *u, t_mini *mini)
{
	if (!str || !u || !mini || !mini->save_ex)
		return (-1);
	if (ft_unset2_init(str, u, mini) == -1)
		return (-1);
	if (ft_unset_path(str, mini) == -1)
		return (ft_free_tab(u->env_supprime), -1);
	while ((mini->save_ex)[u->j + u->supprime] != NULL)
	{
		if (((u->j + u->supprime) < u->taille) && ft_strncmp(str,
				(mini->save_ex)[u->j + u->supprime], u->len_str) == 0
			&& ((mini->save_ex)[u->j + u->supprime][u->len_str] == '='
				|| (mini->save_ex)[u->j + u->supprime][u->len_str] == '\0'))
		{
			u->supprime++;
			continue ;
		}
		u->env_supprime[u->j] = ft_strdup((mini->save_ex)[u->j + u->supprime]);
		if (!u->env_supprime[u->j])
			return (ft_free_tab(u->env_supprime), -1);
		u->j++;
	}
	u->env_supprime[u->j] = NULL;
	ft_free_tab((mini->save_ex));
	return ((mini->save_ex) = u->env_supprime, 0);
}

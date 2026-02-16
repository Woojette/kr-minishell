/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/16 21:12:11 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_update(char *str, t_mini *mini)
{
	int		i;
	char	**result;

	i = -1;
	result = malloc(sizeof(char *) * (ft_env_len(mini->env) + 2));
	if (!result)
		return ;
	while (mini->env[++i])
	{
		if (!(result[i] = ft_strdup(mini->env[i])))
		{
			result[i] = NULL;
			free_tab_char(result);
			return ;
		}
	}
	result[i] = ft_strdup(str);
	if (!result[i])
	{
		free_tab_char(result);
		return ;
	}
	result[++i] = NULL;
	free_tab_char(mini->env);
	mini->env = result;
}

void	env_update_sub(char *str, t_mini *mini, int change_pos)
{
	int		taille;
	char	**new_env;
	int		i;

	i = -1;
	taille = ft_env_len(mini->env);
	new_env = malloc(sizeof(char *) * (taille + 1));
	if (!new_env)
		return ;
	while (++i < taille)
	{
		if (i == change_pos)
			new_env[i] = ft_strdup(str);
		else
			new_env[i] = ft_strdup(mini->env[i]);
		if (!new_env[i])
		{
			new_env[i] = NULL;
			free_tab_char(new_env);
			return ;
		}
	}
	new_env[i] = NULL;
	free_tab_char(mini->env);
	mini->env = new_env;
}

void	env_update2(char *str, t_mini *mini)
{
	int	index;

	if (equal_checker(str) != 1)
		return ;
	index = key_index(str, mini->env);
	if (index < 0)
	{
		env_update(str, mini);
		return ;
	}
	env_update_sub(str, mini, index);
}

void	ex_and_env(char *str, t_mini *mini)
{
	exp_update(str, mini);
	env_update(str, mini);
}

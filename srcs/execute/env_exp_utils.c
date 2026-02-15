/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exp_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:17:16 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/15 18:09:43 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_len(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		i ++;
	}
	return i;
}

void	copy_env(t_mini *mini, char **env)
{
	int	i;
	int	j;
	char **result;
	
	i = -1;
	if (!env)
		return ;
	result = malloc(sizeof(char*) * (ft_env_len(env) + 1));
	if (!result)
		return ;
	while (env[++i])
	{
		result[i] = ft_strdup(env[i]);
		if (!result[i])
		{
			j = 0;
			while (j < i)
				free(result[j++]);
			return (free(result));
		}
	}
	result[i] = NULL;
	mini->env = result;
}

void	copy_to_exp(t_mini *mini, char **env)
{
	int	i;
	int	j;
	char **result;
	
	i = -1;
	if (!env)
		return ;
	result = malloc(sizeof(char*) * (ft_env_len(env) + 1));
	if (!result)
		return ;
	while (env[++i])
	{
		result[i] = ft_strdup(env[i]);
		if (!result[i])
		{
			j = 0;
			while (j < i)
				free(result[j++]);
			return (free(result));
		}
	}
	result[i] = NULL;
	mini->save_ex = result;
}

void	copy_env_exp(t_mini *mini, char **env)
{
	copy_env(mini, env);
	copy_to_exp(mini, env);
	if (!mini->env)
	{
		mini->env = malloc(sizeof(char *));
		if (mini->env)
			mini->env[0] = NULL;
		fatal_error(mini, "malloc fail on env");
	}
		if (!mini->save_ex)
	{
		mini->save_ex = malloc(sizeof(char *));
		if (mini->save_ex)
			mini->save_ex[0] = NULL;
		fatal_error(mini, "malloc fail on export");
	}
}

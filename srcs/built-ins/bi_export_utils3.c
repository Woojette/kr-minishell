/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exp_update(char *str, t_mini *mini)
{
	int		i;
	char	**result;

	i = -1;
	result = malloc(sizeof(char *) * (ft_env_len(mini->save_ex) + 2));
	if (!result)
		return ;
	while (mini->save_ex[++i])
	{
		result[i] = ft_strdup(mini->save_ex[i]);
		if (!result[i])
		{
			free_tab_char(result);
			return ;
		}
	}
	result[i] = ft_strdup(str);
	if (!(result[i]))
	{
		free_tab_char(result);
		return ;
	}
	result[++i] = NULL;
	free_tab_char(mini->save_ex);
	mini->save_ex = result;
}

void	exp_update2(char *str, t_mini *mini, int change_pos)
{
	int taille;
	char **new_exp;
	int i;

	i = 0;
	taille = ft_env_len(mini->save_ex);
	new_exp = malloc(sizeof(char *) * (taille + 1));
	if (!new_exp)
		return ;
	while (i < taille)
	{
		if (i == change_pos)
			new_exp[i] = ft_strdup(str);
		else
			new_exp[i] = ft_strdup(mini->save_ex[i]);
		if (!new_exp[i])
		{
			new_exp[i] = NULL;
			return (free_tab_char(new_exp));
		}
		i++;
	}
	new_exp[i] = NULL;
	free_tab_char(mini->save_ex);
	mini->save_ex = new_exp;
}
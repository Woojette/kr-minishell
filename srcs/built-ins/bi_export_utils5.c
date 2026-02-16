/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export_utils5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	re_ex_and_env(char *str, t_mini *mini)
{
	int	i;
	int	change_pos;

	i = 0;
	change_pos = key_index(str, mini->save_ex);
	exp_update2(str, mini, change_pos);
	env_update2(str, mini);
}

void	save_export(char *str, t_mini *mini)
{
	if (!has_valid_key(str))
	{
		printf("export : not a valid identifier\n");
		mini->exit_status = 1;
		return ;
	}
	else
	{
		mini->exit_status = 0;
		if (has_samekey(str, *mini) == -1)
		{
			if (equal_checker(str) == 1)
				ex_and_env(str, mini);
			else
				exp_update(str, mini);
		}
		else if (has_samekey(str, *mini) == 0)
			return ;
		else if (has_samekey(str, *mini) == 1)
		{
			if (same_checker(str, *mini) == 1)
				return ;
			re_ex_and_env(str, mini);
		}
	}
}

void	bubble_pointer(char **str1, char **str2)
{
	char	*temp;

	temp = *str1;
	*str1 = *str2;
	*str2 = temp;
}

void	export_sort(t_mini *mini)
{
	int		i;
	int		j;
	char	**sorted;

	i = 0;
	sorted = mini->save_ex;
	while (i < ft_env_len(mini->save_ex) - 1)
	{
		j = 0;
		while (j < ft_env_len(mini->save_ex) - 1)
		{
			if (ft_strcmp(sorted[j], sorted[j + 1]) > 0)
				bubble_pointer(&sorted[j], &sorted[j + 1]);
			j++;
		}
		i++;
	}
}

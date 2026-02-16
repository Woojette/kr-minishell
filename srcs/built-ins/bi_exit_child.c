/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit_sans_arg2(t_mini *mini)
{
	free_round(mini);
	printf("exit\n");
	exit(mini->exit_status);
}

void	ft_exit_normal_arg2(long long val, t_mini *mini)
{
	mini->exit_status = val % 256;
	free_round(mini);
	printf("exit\n");
	exit(mini->exit_status);
}

void	ft_exit_wrong_arg2(char *str, t_mini *mini)
{
	mini->exit_status = 2;
	free_round(mini);
	printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", str);
	exit(mini->exit_status);
}

void	ft_exit2(char **tab, t_mini *mini)
{
	long long temp;
	int error;

	error = 0;
	if (tab[1] == NULL)
		ft_exit_sans_arg2(mini);
	else if ((tab[1] != NULL) && (tab[2] != NULL))
	{
		ft_exit_pl_arg(mini);

		return ;
	}
	else if ((tab[1] != NULL) && (tab[2] == NULL))
	{
		if (ft_exit_check_not_int(tab[1]) == 1)
			ft_exit_wrong_arg2(tab[1], mini);
		temp = ft_exit_atoi_long(tab[1], &error);
		if (error == 1)
			ft_exit_wrong_arg2(tab[1], mini);
		ft_exit_normal_arg2(temp, mini);
	}
}

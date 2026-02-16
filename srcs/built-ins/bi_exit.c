/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit_sans_arg(t_mini *mini)
{
	mini->exit_status = mini->exit_status % 256;
	termios_back(mini);
	free_mini(mini);
	printf("exit\n");
	exit(mini->exit_status);
}

void	ft_exit_normal_arg(long long val, t_mini *mini)
{
	mini->exit_status = val % 256;
	termios_back(mini);
	// free_mini(mini);
	printf("exit\n");
	exit(mini->exit_status);
}

void	ft_exit_pl_arg(t_mini *mini)
{
	mini->exit_status = 1;
	printf("exit\n");
	printf("minishell: exit: too many arguments\n");
}

void	ft_exit_wrong_arg(char *str, t_mini *mini)
{
	mini->exit_status = 2;
	termios_back(mini);
	// free_mini(mini);
	printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", str);
	exit(mini->exit_status);
}

void	ft_exit(char **tab, t_mini *mini)
{
	long long	temp;
	int			error;

	error = 0;
	if (tab[1] == NULL)
		ft_exit_sans_arg(mini);
	else if ((tab[1] != NULL) && (tab[2] != NULL))
	{
		ft_exit_pl_arg(mini);
		return ;
	}
	else if ((tab[1] != NULL) && (tab[2] == NULL))
	{
		if (ft_exit_check_not_int(tab[1]) == 1)
			ft_exit_wrong_arg(tab[1], mini);
		temp = ft_exit_atoi_long(tab[1], &error);
		if (error == 1)
			ft_exit_wrong_arg(tab[1], mini);
		ft_exit_normal_arg(temp, mini);
	}
}

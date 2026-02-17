/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 21:17:23 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 21:17:32 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit_sans_arg(t_mini *mini)
{
	int	save_exit_status;

	mini->exit_status = mini->exit_status % 256;
	save_exit_status = mini->exit_status;
	// termios_back(mini);
	// free_mini(mini);
	exit_clean(mini);
	printf("exit\n");
	exit(save_exit_status);
}

void	ft_exit_normal_arg(long long val, t_mini *mini)
{
	int	save_exit_status;

	mini->exit_status = val % 256;
	save_exit_status = mini->exit_status;
	// termios_back(mini);
	// free_mini(mini);
	exit_clean(mini);
	printf("exit\n");
	exit(save_exit_status);
}

void	ft_exit_pl_arg(t_mini *mini)
{
	mini->exit_status = 1;
	printf("exit\n");
	printf("minishell: exit: too many arguments\n");
}

void	ft_exit_wrong_arg(char *str, t_mini *mini)
{
	int	save_exit_status;

	mini->exit_status = 2;
	save_exit_status = mini->exit_status;
	// termios_back(mini);
	// free_mini(mini);
	exit_clean(mini);
	printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", str);
	exit(save_exit_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:41:39 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 01:28:56 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fatal_error(t_mini *mini, char *err)
{
	if (err)
		perror(err);
	else
		perror("Error");
	termios_back(mini);
	free_mini(mini);
	exit(EXIT_FAILURE);
}

void	p_exit(t_mini *mini)
{
	termios_back(mini);
	free_mini(mini);
	exit(mini->exit_status);
}

void	p_exit_nb(t_mini *mini, int nb_exit)
{
	termios_back(mini);
	free_mini(mini);
	exit(nb_exit);
}

void	child_exit(t_mini *mini)
{
	free_round(mini);
	exit(mini->exit_status);
}

void	child_exit_nb(t_mini *mini, int nb_exit)
{
	free_round(mini);
	exit(nb_exit);
}

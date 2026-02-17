/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/14 18:54:23 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_exit(int *exit_status)
{
	static int	*new_exit = NULL;

	if (new_exit == NULL)
		new_exit = exit_status;
	*new_exit = 130;
}

void	appliquer_sigint_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	set_exit(&sig);
}

void	print_heredoc_warning_ctrl_d(char *delimiter)
{
	if (!delimiter)
		delimiter = "";
	write(2, "warning: here-document delimited by end-of-file (wanted '", 57);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
}

void	set_signal_parent_wait(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signal_exec_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

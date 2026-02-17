/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_c.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:41:18 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 01:15:12 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	control_c(t_mini *mini)
{
	struct termios	renew_term;

	tcgetattr(STDIN_FILENO, &mini->orig_term);
	renew_term = mini->orig_term;
	renew_term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &renew_term);
}

void	termios_back(t_mini *mini)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &mini->orig_term);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_refac_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 12:18:21 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/18 21:15:37 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_main(t_mini *mini)
{
	int	check_builtin;

	redirection_center(mini);
	if (mini->cmd_array && mini->cmd_array[0].cmd && mini->cmd_array[0].cmd[0])
	{
		check_builtin = is_built_in(mini->cmd_array[0].cmd[0]);
		if (mini->nbr_cmd == 1 && check_builtin != T_NOT_BUILT_IN)
		{
			one_builtin_avec_redirs(mini);
		}
		else
			fork_center(mini);
	}
	else if (cmd_qqpart(mini))
		fork_center(mini);
}

void	clean_after_exec(t_mini *mini, t_token *parsing, char *line)
{
	if (mini->cmd_array)
		free_cmd_all(mini->cmd_array, mini->nbr_cmd);
	mini->cmd_array = NULL;
	mini->nbr_cmd = 0;
	if (parsing)
		free_tokens(&parsing);
	if (line)
		free(line);
}

void	clean_after_cmd(t_m *m)
{
	free_cmd_all(m->mini->cmd_array, m->mini->nbr_cmd);
	m->mini->cmd_array = NULL;
	m->mini->nbr_cmd = 0;
	free_tokens(&m->parsing);
	free(m->line);
}

int	after_all(t_mini *mini)
{
	termios_back(mini);
	free_mini(mini);
	return (0);
}

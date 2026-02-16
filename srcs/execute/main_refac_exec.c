/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_refac_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 12:18:21 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 22:12:29 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execution_main(t_mini *mini)
{
	int 	check_builtin;

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
		free_cmd_all(mini->cmd_array, mini->nbr_cmd); // 이번 이터레이션 자원만 정리하기
	mini->cmd_array = NULL;
	mini->nbr_cmd = 0;
	if (parsing)
		free_tokens(&parsing);
	if (line)
		free(line);
}

int	after_all(t_mini *mini)
{
	termios_back(mini);
	free_mini(mini);
	return (0);
}

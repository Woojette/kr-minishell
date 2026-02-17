/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_main(int ac, char **av, char **env, t_m *m)
{
	(void)ac;
	(void)av;
	m->line = NULL;
	m->cmd = NULL;
	m->parsing = NULL;
	m->j = 0;
	m->resultat = 0;
	m->nbr_cmd = 0;
	m->mini = ft_calloc(1, sizeof(t_mini));
	if (!m->mini)
		return (-1);
	copy_env_exp(m->mini, env);
	// set_path_array(m->mini);
	m->mini->exit_status = 0;
	m->mini->cmd_array = NULL;
	m->mini->nbr_cmd = 0;
	set_exit(&m->mini->exit_status);
	m->mini->exit_status = 0;
	m->mini->m_ptr = m;
	return (0);
}

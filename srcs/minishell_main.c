/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/16 22:12:07 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

void	init_avant_prompt(t_m *m)
{
	tcgetattr(STDIN_FILENO, &m->mini->orig_term);
	control_c(m->mini);
	init_signaux();
	if (g_exit_status != 0)
	{
		m->mini->exit_status = g_exit_status;
		g_exit_status = 0;
	}
}

int	main(int ac, char **av, char **env)
{
	t_m	*m;

	m = ft_calloc(1, sizeof(t_m));
	if (!m)
		return (-1);
	if (init_main(ac, av, env, m) < 0)
		return (free(m), -1);
	while (1)
	{
		m->resultat = check_parsing(m);
		if (m->resultat == -2)
			break ;
		else if (m->resultat == -1)
			continue ;
		execution_main(m->mini);
		clean_after_exec(m->mini, m->parsing, m->line);
	}
	return (after_all(m->mini));
}

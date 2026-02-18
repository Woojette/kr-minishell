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

int	check_readline(t_m *m)
{
	init_avant_prompt(m);
	m->line = readline("coucou$ ");
	if (!m->line)
	{
		write(1, "exit\n", 5);
		return (-2);
	}
	if (m->line[0] == '\0')
	{
		free(m->line);
		return (-1);
	}
	return (0);
}

int	check_syntaxe(t_m *m)
{
	m->parsing = NULL;
	if (check_erreur_syntaxe(m) == -1)
		return (-1);
	if (check_erreur_parse_input(m) == -1)
		return (-1);
	return (0);
}

int	check_erreur_cmd(t_m *m)
{
	m->cmd = malloc_cmd(m->parsing);
	if (!m->cmd)
	{
		free_tokens(&m->parsing);
		free(m->line);
		return (-1);
	}
	m->nbr_cmd = count_pipe(m->parsing) + 1;
	m->resultat = add_cmd(m->parsing, m->cmd);
	if (m->resultat == -1)
	{
		erreur_cmd(m);
		return (-2);
	}
	else if (m->resultat == -2)
	{
		erreur_cmd(m);
		return (-1);
	}
	m->mini->cmd_array = m->cmd;
	m->mini->nbr_cmd = m->nbr_cmd;
	m->j = 0;
	return (0);
}

int	check_cmd_heredoc(t_m *m)
{
	while (m->j < m->mini->nbr_cmd)
	{
		if (appliquer_heredoc_cmd(m->mini, m->j) < 0)
			return (-2);
		m->j++;
	}
	return (0);
}

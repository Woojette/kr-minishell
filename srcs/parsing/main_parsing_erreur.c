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

void	erreur_syntaxe(t_m *m, int code)
{
	if (code == 0)
	{
		write(2, "Error: unclosed quotes\n", 23);
		m->mini->exit_status = 2;
		free(m->line);
	}
	else if (code == 1)
	{
		write(2, "Error: syntax error near unexpected token `|'\n", 47);
		m->mini->exit_status = 2;
		free(m->line);
	}
	return ;
}

void	erreur_parse(t_m *m)
{
	if (m->resultat == -2)
		m->mini->exit_status = 2;
	else
		m->mini->exit_status = 1;
	free_tokens(&m->parsing);
	free(m->line);
}

void	erreur_cmd(t_m *m)
{
	free_cmd_all(m->cmd, m->nbr_cmd);
	free_tokens(&m->parsing);
	free(m->line);
}

int	check_erreur_syntaxe(t_m *m)
{
	if (check_quotes(m->line) == 0)
	{
		erreur_syntaxe(m, 0);
		return (-1);
	}
	if (check_pipe_fin(m->line) == 1)
	{
		erreur_syntaxe(m, 1);
		return (-1);
	}
	return (0);
}

int	check_erreur_parse_input(t_m *m)
{
	m->resultat = parse_input(m->line, &m->parsing, m->mini);
	if (m->resultat < 0)
	{
		erreur_parse(m);
		return (-1);
	}
	return (0);
}

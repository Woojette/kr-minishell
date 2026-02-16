/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

int	init_main(int ac, char **av, char **env, t_m *m)
{
	(void)ac;
	(void)av;
	m->line = NULL;
	m->cmd = NULL;
	m->parsing = NULL;
	m->j = 0;
	// m->check_builtin = 0;
	m->resultat = 0;
	m->nbr_cmd = 0;
	m->mini = ft_calloc(1, sizeof(t_mini));
	if (!m->mini)
		return (-1);
	copy_env_exp(m->mini, env);
	set_path_array(m->mini);
	m->mini->exit_status = 0;
	m->mini->cmd_array = NULL;
	m->mini->nbr_cmd = 0;
	return (0);
}

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
		init_avant_prompt(m);
		m->line = readline("coucou$ ");
		if (!m->line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (m->line[0] == '\0')
		{
			free(m->line);
			continue ;
		}
		add_history(m->line);
		m->parsing = NULL;
		if (check_quotes(m->line) == 0)
		{
			write(2, "Error: unclosed quotes\n", 23);
			m->mini->exit_status = 2;
			free(m->line);
			continue ;
		}
		if (check_pipe_fin(m->line) == 1)
		{
			write(2, "Error: syntax error near unexpected token `|'\n", 47);
			m->mini->exit_status = 2;
			free(m->line);
			continue ;
		}
		m->resultat = parse_input(m->line, &m->parsing, m->mini);
		if (m->resultat < 0)
		{
			if (m->resultat == -2)
				m->mini->exit_status = 2;
			else
				m->mini->exit_status = 1;
			free_tokens(&m->parsing);
			free(m->line);
			continue ;
		}
		m->cmd = malloc_cmd(m->parsing);
		if (!m->cmd)
		{
			free_tokens(&m->parsing);
			free(m->line);
			continue ;
		}
		m->nbr_cmd = count_pipe(m->parsing) + 1;
		m->resultat = add_cmd(m->parsing, m->cmd);
		if (m->resultat == -1)
		{
			free_cmd_all(m->cmd, m->nbr_cmd);
			free_tokens(&m->parsing);
			free(m->line);
			continue ;
		}
		else if (m->resultat == -2)
		{
			free_cmd_all(m->cmd, m->nbr_cmd);
			free_tokens(&m->parsing);
			free(m->line);
			continue ;
		}
		m->mini->cmd_array = m->cmd;
		m->mini->nbr_cmd = m->nbr_cmd;
		m->j = 0;
		while (m->j < m->mini->nbr_cmd)
		{
			if (appliquer_heredoc_cmd(m->mini, m->j) < 0)
				break ;
			m->j++;
		}
		if (m->j < m->mini->nbr_cmd)
		{
			free_cmd_all(m->mini->cmd_array, m->mini->nbr_cmd);
			m->mini->cmd_array = NULL;
			m->mini->nbr_cmd = 0;
			free_tokens(&m->parsing);
			free(m->line);
			continue ;
		}
		if (m->j < m->mini->nbr_cmd)
		{
			free_cmd_all(m->mini->cmd_array, m->mini->nbr_cmd);
			m->mini->cmd_array = NULL;
			m->mini->nbr_cmd = 0;
			free_tokens(&m->parsing);
			free(m->line);	
			continue ;
		}
		execution_main(m->mini);
		clean_after_exec(m->mini, m->parsing, m->line);
	}
	return (after_all(m->mini));
}

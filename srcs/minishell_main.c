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

int	init_main(int ac, char **av, char **env, t_m *m)
{
	char			*line;
	t_cmd			*cmd;
	t_token			*parsing;
	int				j;
	int				resultat;
	int				nbr_cmd;
	t_mini			*mini;
	struct termios	orig_term;

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

int	check_parse(t_m *m)
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
	if (m->j < m->mini->nbr_cmd)
	{
		clean_after_cmd(m);
		return (-1);
	}
	return (0);
}

// int	check_main(m)
// {
// 		m->resultat = check_readline(m);
// 		if (m->resultat == -2)
// 			break ;
// 		else if (m->resultat == -1)
// 			continue ;
// 		add_history(m->line);
// 		m->resultat = check_parse(m);
// 		if (m->resultat == -1)
// 			continue ;
// 		m->resultat = check_erreur_cmd(m);
// 		if (m->resultat == -1)
// 			continue ;
// 		else if (m->resultat == -2)
// 			break ;
// 		m->resultat = check_cmd_heredoc(m);
// 		if (m->resultat == -1)
// 			continue ;
// 		else if (m->resultat == -2)
// 			break ;
// 		execution_main(m->mini);
// 		clean_after_exec(m->mini, m->parsing, m->line);
// }

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
		m->resultat = check_readline(m);
		if (m->resultat == -2)
			break ;
		else if (m->resultat == -1)
			continue ;
		add_history(m->line);
		m->resultat = check_parse(m);
		if (m->resultat == -1)
			continue ;
		m->resultat = check_erreur_cmd(m);
		if (m->resultat == -1)
			continue ;
		else if (m->resultat == -2)
			break ;
		m->resultat = check_cmd_heredoc(m);
		if (m->resultat == -1)
			continue ;
		else if (m->resultat == -2)
			break ;
		execution_main(m->mini);
		clean_after_exec(m->mini, m->parsing, m->line);
	}
	return (after_all(m->mini));
}

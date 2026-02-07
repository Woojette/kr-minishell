#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char		*line;
	t_cmd		*cmd;
	t_token	*parsing;
	// t_token	*temp;
	// int			i;
	int			j;
	int			resultat;
	int			nbr_cmd;
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (0);
	mini->env = env;
	mini->exit_status = 0;
	mini->cmd = NULL;
	mini->nbr_cmd = 0;
	// i = 0;
	cmd = NULL;
	j = 0;
	while (1)
	{
		init_signaux();
		mini->exit_status = 0;
		line = readline("coucou$ ");
		if (!line) // ctrl D
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		// printf("Input line: %s\n", line);
		add_history(line);

		parsing = NULL;
		// i = 0;
		if (check_quotes(line) == 0)
		{
			write(2, "Error: unclosed quotes\n", 23);
			free(line);
			continue ;
		}
		if (check_pipe_fin(line) == 1)
		{
			write(2, "Error: syntax error near unexpected token '|'\n", 47);
			free(line);
			continue ;
		}
		if (parse_input(line, &parsing, mini) < 0)
		{
			write(2, "Error: parse_input failed\n", 26);
			free_tokens(&parsing);
			free(line);
			continue ;
		}
		
		// temp = parsing;
		// while (temp)
		// {
		// 	printf("testing tokens:\n");
		// 	printf("noeud %d '%s' | type %s | type_quote %s\n", i, temp->str, get_token_type_str(temp->type_token), get_token_type_state(temp->type_quote));
		// 	i++;
		// 	temp = temp->next;
		// }
		cmd = malloc_cmd(parsing);
		if (!cmd)
		{
			free_tokens(&parsing);
			free(line);
			continue ;
		}
		nbr_cmd = count_pipe(parsing) + 1;
		resultat = add_cmd(parsing, cmd);
		if (resultat == -1)
		{
			free_cmd_all(cmd, nbr_cmd);
			free_tokens(&parsing);
			free(line);
			continue ;
		}
		else if (resultat == -2)
		{
			free_cmd_all(cmd, nbr_cmd);
			free_tokens(&parsing);
			free(line);
			continue ;
		}
		mini->cmd = cmd;
		mini->nbr_cmd = nbr_cmd;
		j = 0;
		while (j < mini->nbr_cmd)
		{
			if (appliquer_heredoc_cmd(mini, j) < 0)
			{
				break ;
			}
			j++;
		}
		if (j < mini->nbr_cmd)
		{
			free_cmd_all(mini->cmd, mini->nbr_cmd);
			mini->cmd = NULL;
			mini->nbr_cmd = 0;
			free_tokens(&parsing);
			free_mini(mini);
			continue ;
		}

		test_print_cmds(cmd, nbr_cmd);
		test_redirs(mini);

		if (mini->cmd)
			free_cmd_all(mini->cmd, mini->nbr_cmd);
		mini->cmd = NULL;
		mini->nbr_cmd = 0;
		if (parsing)
			free_tokens(&parsing);
		if (line)
			free(line);
	}
	free_mini(mini);
	return (0);
}

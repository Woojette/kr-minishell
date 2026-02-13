#include "minishell.h"

int			g_exit_status = 0;


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
	int 	check_builtin;
	int			resultat;
	int			nbr_cmd;
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (0);
	copy_env(mini, env); //보이드로 바로 복사체 스트럭트에 업뎃
	set_path_array(mini);
	mini->exit_status = 0;
	mini->cmd_array = NULL;
	mini->nbr_cmd = 0;
	// i = 0;
	cmd = NULL;
	j = 0;

	while (1)
	{
		init_signaux();
		mini->exit_status = g_exit_status;
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
		mini->cmd_array = cmd;
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
			free_cmd_all(mini->cmd_array, mini->nbr_cmd);
			mini->cmd_array = NULL;
			mini->nbr_cmd = 0;
			free_tokens(&parsing);
			free_mini(mini);
			continue ; // on a fait free mini tout a l'heure, ca risque d'utiliser mini a la prochaine iteration, alors on break pour sortir de la boucle et faire free_mini a la fin du main
      // a faire : free mini, ou faire un return 0 pour sortir du main, ou faire un break pour sortir de la boucle et faire free_mini a la fin du main *********************************
		}
		// test_print_cmds(cmd, nbr_cmd);
		// test_redirs(mini);
		redirection_center(mini);
		if (mini->cmd_array && mini->cmd_array[0].cmd && mini->cmd_array[0].cmd[0])
		{
			check_builtin = is_built_in(mini->cmd_array[0].cmd[0]);
    		if (mini->nbr_cmd == 1 && check_builtin != T_NOT_BUILT_IN)
    		{
				one_builtin_avec_redirs(mini);
			// 함수 1
				// 1개 실행을 위한 리다이렉션 체크 + 호출 함수 호출,
				//dup2 
      		// execute_built_in(mini, mini->cmd_array[0].cmd, check_builtin);
			// 함수 2
				// stdin, out 으로 복원 
			// return (0);
    		} 
			else 
    		fork_center(mini);
		}
		else if (cmd_qqpart(mini))
			fork_center(mini);
		if (mini->cmd_array)
			free_cmd_all(mini->cmd_array, mini->nbr_cmd); // 이번 이터레이션 자원만 정리하기
		mini->cmd_array = NULL;
		mini->nbr_cmd = 0;
		if (parsing)
			free_tokens(&parsing);
		if (line)
			free(line);
	}
	free_mini(mini);
	return (0);
}

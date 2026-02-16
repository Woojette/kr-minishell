#include "minishell.h"

int			g_exit_status = 0;

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char		*line;
	t_cmd		*cmd;
	t_token	*parsing;
	int			j;
	int 	check_builtin;
	int			resultat;
	int			nbr_cmd;
	t_mini	*mini;
	struct termios orig_term;

	mini = ft_calloc(1, sizeof(t_mini)); // memset 0 더해서 쓰레기값 방지
	if (!mini)
		return (0);
	// mini->pipe_read_end = -1;
	copy_env_exp(mini, env); //보이드로 바로 복사체 스트럭트에 업뎃
	set_path_array(mini);
	mini->exit_status = 0;
	mini->cmd_array = NULL;
	mini->nbr_cmd = 0;
	// i = 0;
	cmd = NULL;
	j = 0;

	while (1)
	{
		tcgetattr(STDIN_FILENO, &orig_term);
		control_c(mini); // 일요일 수정함
		init_signaux();
		if (g_exit_status != 0)
		{
			mini->exit_status = g_exit_status;
			g_exit_status = 0;
		}
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
		add_history(line);

		parsing = NULL;
		// i = 0;
		if (check_quotes(line) == 0)
		{
			write(2, "Error: unclosed quotes\n", 23);
			mini->exit_status = 2;
			free(line);
			continue ;
		}
		if (check_pipe_fin(line) == 1)
		{
			write(2, "Error: syntax error near unexpected token '|'\n", 47);
			mini->exit_status = 2;
			free(line);
			continue ;
		}
		if (parse_input(line, &parsing, mini) < 0)
		{
			write(2, "Error: parse_input failed\n", 26);
			mini->exit_status = 1;
			free_tokens(&parsing);
			free(line);
			continue ;
		}

		// print_tokens(parsing);

		cmd = malloc_cmd(parsing);
		if (!cmd)
		{
			free_tokens(&parsing);
			free(line);
			continue ;
		}
		// printf("cmd\n");
		nbr_cmd = count_pipe(parsing) + 1;
		resultat = add_cmd(parsing, cmd);
		// printf("add cmd resultat: %d\n", resultat);
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

		// printf("cmd2\n");
		// print_cmd_array(mini->cmd_array, mini->nbr_cmd);

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
			//??!!??!!free(line) 잇어야 된다는데 그게 머지? free(line); line = NULL;
			// continue 때문에 점프해서 프리 안함
			// free(line);
			// free_mini(mini); //  종료할 때만 해야함
			continue ;

		}
	
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
	termios_back(mini);
	free_mini(mini);
	return (0);
}

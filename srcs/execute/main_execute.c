#include "exec_run.h"

// echo hello | cat -e > out1 > out2

// 일어나서 할 거 -> 디렉토리 체크 함수(stat 써서 실패 분기 잘하기), 
// 빌트인 체크 함수 - 파이프 없을때 빌트인 부모 프로세스 실행 / 파이프 있을 때 빌트인 자식에서 실행
// 인자 char **cmd 랑 내부의 cmd->cmd[0] cmd_path_center(mini, cmd) 등 난리난거고치기

char	*find_path(t_mini *mini)
{
	int	i;
	i = 0;
	if (!mini->env)
		return (NULL);
	while (mini->env[i])
	{
		if (ft_strncmp(mini->env[i], "PATH=", 5) == 0)
			return (mini->env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*get_cmd(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	*tmp;
	char	*full;
	i = 0;

	if (!mini || !mini->cmd_paths || !cmd || !cmd->cmd || !cmd->cmd[0])
		return (NULL);
	while (mini-> cmd_paths[i])
	{
		tmp = ft_strjoin(mini->cmd_paths[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd->cmd[0]);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full); // caller to free
		free(full);
		i++;
	}
	return (NULL);
}

void	get_cmd_paths(t_mini *mini)
{
	char	*env_path;

	env_path = find_path(mini);
	if (!env_path)
		msg_error(mini, "PATH not found");
	mini->paths = ft_strdup(env_path);
	if (!mini->paths)
		msg_error(mini, "malloc error on PATH copy");
	mini->cmd_paths = ft_split(mini->paths, ':');
	if (!mini->cmd_paths)
		msg_error(mini, "malloc error on PATH split");
}

// !!!! cmd->cmd[0] 에 / 가 들어있을 경우 PATH 탐색 없이 그 경로로 바로 검사

char	*cmd_path_center(t_mini *mini, t_cmd *cmd)
{
	1) 빌트인 처리
	2) 실행할 실제 경로 말록해서 리턴하거나 NULL
	// if (ft_strchr(cmd->cmd[0], '/'))
	// {
	// 	if (access(cmd->cmd[0], X_OK) == 0)
	// 		return (ft_strdup(cmd->cmd[0]));
	// 	// return (NULL);
	// }
	get_cmd_paths(mini);
	if (dir error)
	{
		126
	}
	if (permission error)
	{
		127
	}
}

void	ft_execute(t_mini *mini, char **cmd)
{
	char	*cmd;
	char	*cmd_path;

	// cmd = options[0];
	// if (!cmd)
	// 	exit(0);
	// if (is_builtin(cmd))
	// 	exit(ft_builtin(options, data));
	cmd_path_center(mini, cmd)
	if (mini->cmd_paths)
	{
	 	// g_exit_status = 127;
	 	// error_command_msg(cmd, CMD_ERROR);
	 	return ;
	}
	// if (is_directory(cmd_path))
	// {
	// 	error_command_msg(cmd, DIREC_ERROR);
	// 	exit(126);
	// }
	execve(cmd[0], cmd->cmd, mini->env); // 주소를 찾아가서(=참조해서 // 인자가 미니의 주소) 컨텐츠를 .으로 열어보는 행위 자체가 화살표다
	// {
	perror(cmd[0]);
	if (errno == ENOENT)
		exit(127);
	exit(126);
	// }
}

void	child_center(t_mini *mini, t_cmd cmd, int *pipe_fd, int i)
{
	printf("cmd.cmd[0] : %s\n", cmd.cmd[0]);
	// g_exit_status = 0;
	// signal_setting_commandmode();
	if (mini->nbr_cmd > 1)
	{
		if (i == 0)
		{
			// 첫번째 커맨드 패밀리
			close(pipe_fd[0]); // Close unused readend
			dup2(pipe_fd[1], 1); // first argument will be useless, cuz it will be camouflaged
			close(pipe_fd[1]); // Close the first argument of dup2
			// mini->pipe_read_end = pipe_fd[0]; // Save read end for the next command
		}
		else if (i != 0 && i != mini->nbr_cmd - 1)
		{
			// 중간 커맨드 패밀리
			dup2(mini->pipe_read_end, 0); // Camouflage saved read end to 0
			close(mini->pipe_read_end); // first argument of dup2 useless close
			dup2(pipe_fd[1], 1);
			close(pipe_fd[1]);
			// mini->pipe_read_end = pipe_fd[0];
			close(pipe_fd[0]);
		}
		else
		{
			// 마지막 커맨드 패밀리
			dup2(mini->pipe_read_end, 0);
			close(mini->pipe_read_end);
		}
	}
	if (mini->cmd_array[i].fd_in != -1)
	{
		dup2(mini->cmd_array[i].fd_in, STDIN_FILENO);
		close(mini->cmd_array[i].fd_in);
	}
	if (mini->cmd_array[i].fd_out != -1)
	{
		dup2(mini->cmd_array[i].fd_out, STDOUT_FILENO);
		close(mini->cmd_array[i].fd_out);
	}
	// 	g_exit_status = 1;
	// if (g_exit_status == 0)
		ft_execute(mini, mini->cmd_array[i].cmd);
	// exit(g_exit_status);
  exit(126);
}

void parent_center(t_mini *mini, int pipe_fd[2], int i)
{
	// if (i == 0)
	// {
	// 	close(pipe_fd[1]);
	// 	mini->pipe_read_end = pipe_fd[0];
	// }
	if (i < mini->nbr_cmd - 1)
	{
		if (mini->pipe_read_end != -1)
		{	
			close(mini->pipe_read_end);
			mini->pipe_read_end = pipe_fd[0];
		}
		else
		{	
			mini->pipe_read_end = pipe_fd[0];
		}
		close(pipe_fd[1]);
	}
	else
	{
		if (mini->pipe_read_end != -1)
			close(mini->pipe_read_end);
	}
}

void  exec_run(t_mini *mini)
{
  int i;
  int pipe_fd[2];
  pid_t child_id;
  mini->pipe_read_end = -1;
  i = -1;
  while(++i < mini->nbr_cmd)
  {
    printf("i : %d\n", i);
    if (i < mini->nbr_cmd - 1) // 미니가 포인터이니 화살표(함수 인자기준)
		pipe(pipe_fd);
    child_id = fork();
    printf("child_id : %d\n", child_id);
    if (child_id == 0)
      child_center(mini, mini->cmd_array[i], pipe_fd, i);
    else
      parent_center(mini, pipe_fd, i);
  }
  while (wait(NULL) > 0)
  	;
}

int	redirection_center(t_mini *mini)
{
	int	i;

	i = 0;
	while(i < mini->nbr_cmd)
	{
		if (mini->cmd_array[i].heredoc)
		{
			if (appliquer_heredoc_cmd(mini, i) == -1)
				return (-1);
		}
		else if (mini->cmd_array[i].outfile && mini->cmd_array[i].outfile[0])
		{
			if (appliquer_infile(mini, i) == -1)
				return (-1);
		}
		if (mini->cmd_array[i].outfile && mini->cmd_array[i].outfile[0])
			process_out_redir(mini, i);
		if (mini->cmd_array[i].in_fail || mini->cmd_array[i].out_fail)
			return (-1);
		i++;
	}
	return (0);
}

int main(void)
{
  t_mini *mini= build_echo_cat_wc();
  printf("finish cmd family\n");
  redirection_center(mini);
  cmd_path_center(mini, cmd); // 헷갈리쓰
  exec_run(mini);

  // 1. 파이프 작동 시키기
  // 2. 리다이랙션 중 out, append, in 작동시키기
  // 3. heredoc
  
  (void)(mini);
}
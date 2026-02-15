#include "exec_run.h"

void	child_center(t_mini *mini, t_cmd cmd, int *pipe_fd, int i)
{
	printf("cmd.cmd[0] : %s\n", cmd.cmd[0]);
	// g_exit_status = 0;
	// signal_setting_commandmode();
	// if (mini->cmd_array[i].in_fail || mini->cmd_array[i].out_fail)
 	// {
	// 	if (pipe_fd[0] >= 0)
	// 		close(pipe_fd[0]);
	// 	if (pipe_fd[1] >= 0)
	// 		close(pipe_fd[1]);
	// 	if (mini->pipe_read_end >= 0)
	// 		close(mini->pipe_read_end);
	// 	exit(mini->exit_status ? mini->exit_status : 1);
  	// }
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
	// if (mini->cmd_array[i].fd_in != -1)
	// {
	// 	dup2(mini->cmd_array[i].fd_in, STDIN_FILENO);
	// 	close(mini->cmd_array[i].fd_in);
	// }
	// if (mini->cmd_array[i].fd_out != -1)
	// {
	// 	dup2(mini->cmd_array[i].fd_out, STDOUT_FILENO);
	// 	close(mini->cmd_array[i].fd_out);
	// }
	// 	g_exit_status = 1;
	// if (g_exit_status == 0)
	ft_execute(mini, &mini->cmd_array[i]);
	// exit(g_exit_status);
	if (pipe_fd[0] >= 0)
		close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	if (mini->pipe_read_end >= 0)
		close(mini->pipe_read_end);
  	exit(126);
}

void parent_center(t_mini *mini, int pipe_fd[2], int i)
{
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

void  fork_center(t_mini *mini)
{
  int i;
  int pipe_fd[2];
  pid_t child_id;
  int	check;

  mini->pipe_read_end = -1;
  i = 0;
  while(i < mini->nbr_cmd)
  {
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
    if (i < mini->nbr_cmd - 1) // 미니가 포인터이니 화살표(함수 인자기준)
	{
		check = pipe(pipe_fd);
		if (check == -1)
			fatal_error(mini, "pipe_error");
	}
    child_id = fork();
	if (child_id < 0)
		fatal_error(mini, "fork_error");
    else if (child_id == 0)
      child_center(mini, mini->cmd_array[i], pipe_fd, i);
	else
      parent_center(mini, pipe_fd, i);
	i++;
  }
 // waitpid 로 엑싯 스테이터스 마지막 커맨드 받기로 바꾸기
  	;
}

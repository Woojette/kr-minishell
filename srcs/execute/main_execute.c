#include "exec_run.h"

// echo hello | cat -e > out1 > out2

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
			mini->pipe_read_end = pipe_fd[0]; // Save read end for the next command
		}
		else if (i != 0 && i != mini->nbr_cmd - 1)
		{
			// 중간 커맨드 패밀리
			dup2(mini->pipe_read_end, 0); // Camouflage saved read end to 0
			close(mini->pipe_read_end); // first argument of dup2 useless close
			dup2(pipe_fd[1], 1);
			close(pipe_fd[1]);
			mini->pipe_read_end = pipe_fd[0];
			close(pipe_fd[0]);
		}
		else
		{
			// 마지막 커맨드 패밀리
			dup2(mini->pipe_read_end, 0);
			close(mini->pipe_read_end);
		}
	}
	// if (redirection_center(pipeline->cmd_block->redirect))
	// 	g_exit_status = 1;
	// if (g_exit_status == 0)
	// 	ft_execute(pipeline->cmd_block->cmd, data);
	// exit(g_exit_status);
  exit(1);
}

void parent_center(t_mini *mini)
{

}

void  exec_run(t_mini *mini)
{
  int i;
  int pipe_fd[2];
  pid_t child_id;
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
      parent_center(mini);
  }
}

int main(void)
{
  t_mini *mini= build_echo_cat_wc();
  printf("finish cmd family\n");
  exec_run(mini);

  // 1. 파이프 작동 시키기
  // 2. 리다이랙션 중 out, append, in 작동시키기
  // 3. heredoc
  
  (void)(mini);
}
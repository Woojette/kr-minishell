#include "exec_run.h"

// echo hello | cat -e > out1 > out2

void	child_center(t_mini *mini, t_cmd *cmd, int *pipe_fd, int i)
{
	// g_exit_status = 0;
	// signal_setting_commandmode();
	// if (data->process_number > 1)
	// {
	// 	if (i == 0)
	// 	{
	// 		// 첫번째 커맨드 패밀리
	// 		ft_dup2(p_fd[1], 1);
	// 		data->prev_fd = p_fd[0];
	// 		ft_close(p_fd[0]);
	// 		ft_close(p_fd[1]);
	// 	}
	// 	else if (i != 0 && i != data->process_number - 1)
	// 	{
	// 		// 중간 커맨드 패밀리
	// 		ft_dup2(data->prev_fd, 0);
	// 		ft_close(data->prev_fd);
	// 		ft_dup2(p_fd[1], 1);
	// 		data->prev_fd = p_fd[0];
	// 		ft_close(p_fd[0]);
	// 		ft_close(p_fd[1]);
	// 	}
	// 	else
	// 	{
	// 		// 마지막 커맨드 패밀리
	// 		ft_dup2(data->prev_fd, 0);
	// 		ft_close(data->prev_fd);
	// 		ft_close(p_fd[0]);
	// 		ft_close(p_fd[1]);
	// 	}
	// }
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
    printf("%d <- i count\n", i);
    if (mini->nbr_cmd > 1)
      pipe(pipe_fd);
    child_id = fork();
    printf("%d is my babyid\n", child_id);
    if (child_id == 0)
      child_center(mini, mini->cmd_array, pipe_fd, i);
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
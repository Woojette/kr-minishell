#include "exec_run.h"
#include <stdio.h>

// echo hello | cat -e > out1 > out2

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
    // child_center();
    printf("%d is my babyid\n", child_id);
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
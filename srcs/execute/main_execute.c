#include "exec_run.h"

int main(int ac, char** av, char** env)
{
    t_mini *mini= build_echo_cat_wc();
    int check_builtin;

    (void)ac;
    (void)av;
    mini->env = env;
    //   redirection_center(mini);
    set_path_array(mini);
    check_builtin = is_built_in(mini->cmd_array[0].cmd[0]);
    printf("cmd0=%s, builtin=%d\n", mini->cmd_array[0].cmd[0], check_builtin);
    if (mini->nbr_cmd == 1 && check_builtin != T_NOT_BUILT_IN)
    {
      execute_built_in(mini, mini->cmd_array[0].cmd, check_builtin);
      printf("happy : %s\n", *mini->cmd_array[0].cmd);
      char buf[1024];
      if (getcwd(buf, sizeof(buf)))
        printf("after cd cwd = %s\n", buf);
      else
      perror("getcwd");

      return (0);
    }
    fork_center(mini);
}
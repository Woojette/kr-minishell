#include "exec_run.h"

int main(int ac, char** av, char** env)
{
    t_mini *mini= build_echo_cat_wc();
    int check_builtin;

    (void)ac;
    (void)av;
     copy_env(mini, env);
    //   redirection_center(mini);
    set_path_array(mini);
    check_builtin = is_built_in(mini->cmd_array[0].cmd[0]);
    if (mini->nbr_cmd == 1 && check_builtin != T_NOT_BUILT_IN)
    {
      execute_built_in(mini, mini->cmd_array[0].cmd, check_builtin);

      return (0);
    }
    fork_center(mini);
}
#include "exec_run.h"

int main(int ac, char** av, char** env)
{
  t_mini *mini= build_echo_cat_wc();

  int	i = 0;
  mini->env = env;
//   redirection_center(mini);
  set_path_array(mini);
//   mini->path_array
	(void)ac;
	(void)av;
while (mini->path_array[i])
{	
	printf("path_array : %s\n", mini->path_array[i]);
	i++;
}
fork_center(mini);

(void)(mini);
}
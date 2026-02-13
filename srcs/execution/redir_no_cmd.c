#include "minishell.h"

int	exec_judge(t_mini *mini, int i)
{
	inout_redir(mini, i);
	if (mini->cmd_array[i].inout_fail)
		mini->exit_status = 0;
	if (!mini->cmd_array || !mini->cmd_array[0].cmd || !mini->cmd_array[0].cmd[0])
		return (1);
}


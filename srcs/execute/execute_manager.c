#include "exec_run.h"

// ls -l | cat -e

void	ft_execute(t_mini *mini, t_cmd *cmd)
{
	char	*valid_cmd;

	if (!cmd || !cmd->cmd || !cmd->cmd[0])
		exit(0);
	// valid_cmd = cmd_path_center(mini, cmd->cmd[0]);
	valid_cmd = cmd->cmd[0];
	printf("valid_cmd: %s\n", valid_cmd);
	// if (!valid_cmd)
	// {
	// 	exec_exit(mini->exit_status ? mini->exit_status : 127);
	// }
	// if (mini->path_array)
	// {
	//  	mini->exit_status = 127;
	//  	return ;
	// }
	// if (im_directory(mini, valid_cmd))
	// {
	// 	exec_exit(mini->exit_status ? mini->exit_status : 126);
	// }
	execve(valid_cmd, cmd->cmd, mini->env);
	free(valid_cmd);
	perror(cmd->cmd[0]);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}
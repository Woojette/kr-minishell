#include "minishell.h"

// ls -l | cat -e

void	invalid_cmd(t_mini *mini, char *not_cmd)
{
	int	len;

	len = ft_strlen(not_cmd);
	if (mini->exit_status == 0)
		mini->exit_status = 127;
	if (mini->exit_status == 127)
	{
		write(2, not_cmd, len);
		write(2, ": command not found\n", 21);
	}
	exit(mini->exit_status);
}

void	ft_execute(t_mini *mini, t_cmd *cmd)
{
	char	*valid_cmd;

	if (cmd->inout_fail)
		return ; // exit 몇인지 확인하기
	if (!cmd || !cmd->cmd || !cmd->cmd[0])
		exit (0);
	// exit(0);
	valid_cmd = cmd_path_center(mini, cmd->cmd[0]);//실행가능한 절대경로 받기
	// valid_cmd = cmd->cmd[0];
	// valid_cmd 가 NULL 인지 확인.
	// printf("valid_cmd: %s\n", valid_cmd);
	if (!valid_cmd)
		invalid_cmd(mini, cmd->cmd[0]);
	execve(valid_cmd, cmd->cmd, mini->env);
	free(valid_cmd);
	perror(cmd->cmd[0]);
	if (errno == ENOENT) // 커맨드 못 찾은 경우 127, 126은 커맨드는 찾음 근데 실행 못함
		exit(127);
	exit(126);
}
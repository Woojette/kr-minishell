
// void	no_path(t_mini *mini, char *no_path) // 패쓰지운 ls 가 여기로 빠지는 거 같은디
// {
// 	int	len;

// 	len = ft_strlen(no_path);
// 	mini->exit_status = 127; // unset 으로 날려버린 ls 랑 happy 랑 같이 떨어짐
// 	write(2, no_path, len);
// 	write(2, ": No such file or directory\n", 28);
// 	child_exit(mini);
// }

// int	path_deleted()

// char	*get_env_path(t_mini *mini)
// {
// 	int	i;
// 	static char del[] = "del";
// 	i = 0;
// 	if (!mini->env)
// 		return (NULL);
// 	while (mini->env[i])
// 	{
// 		if (ft_strncmp(mini->env[i], "PATH=", 5) == 0)
// 			return (mini->env[i] + 5);
// 		i++;
// 	}
// 	mini----->flag = 1;
// 	return (NULL); // 여기서 원래 env 에 존재하는 애면 하드코딩으로 문자 
// }

// #include "minishell.h"

// void	invalid_cmd(t_mini *mini, char *not_cmd)
// {
// 	int	len;

// 	len = ft_strlen(not_cmd);
// 	mini->exit_status = 127;
// 	if (mini->no_path == 0)
// 	{
// 		write(2, not_cmd, len);
// 		write(2, ": command not found\n", 21);
// 		child_exit(mini);
// 	}
// 	if (mini->no_path == 1)
// 	{
// 		write(2, not_cmd, len);
// 		write(2, ": No such file or directory\n", 28);
// 		child_exit(mini);
// 	}
// }

// void	ft_execute(t_mini *mini, t_cmd *cmd)
// {
// 	char	*valid_cmd;

// 	if (cmd->inout_fail)
// 		child_exit_nb(mini, 1);
// 	if (!cmd || !cmd->cmd || !cmd->cmd[0])
// 		child_exit_nb(mini, 0);
// 	valid_cmd = cmd_path_center(mini, cmd->cmd[0]);
// 	if (!valid_cmd)
// 	{
// 		if (mini->exit_status == 126)
// 			child_exit_nb(mini, 126);
// 		invalid_cmd(mini, cmd->cmd[0]);
// 	}
// 	execve(valid_cmd, cmd->cmd, mini->env);
// 	free(valid_cmd);
// 	perror(cmd->cmd[0]);
// 	if (errno == ENOENT)
// 		child_exit_nb(mini, 127);
// }
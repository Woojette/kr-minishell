#include "exec_run.h"

char	*get_env_path(t_mini *mini)
{
	int	i;
	i = 0;
	if (!mini->env)
		return (NULL);
	while (mini->env[i])
	{
		if (ft_strncmp(mini->env[i], "PATH=", 5) == 0)
			return (mini->env[i] + 5);
		i++;
	}
	return (NULL);
}

void	set_path_array(t_mini *mini)
{
	char	*env_path;

	if (mini->path_array)
		return ;
	env_path = get_env_path(mini);
	if (!env_path)
		msg_error(mini, "PATH not found");
	mini->path_array = ft_split(env_path, ':');
	if (!mini->path_array)
		msg_error(mini, "malloc error on PATH split");
}


char	*get_cmd(t_mini *mini, t_cmd *cmd)
{
	int		i;
	char	*tmp;
	char	*full;
	i = 0;

	if (!mini || !mini->path_array || !cmd || !cmd->cmd || !cmd->cmd[0])
		return (NULL);
	while (mini-> path_array[i])
	{
		tmp = ft_strjoin(mini->path_array[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd->cmd[0]);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full); // caller to free
		free(full);
		i++;
	}
	return (NULL);
}
// !!!! cmd->cmd[0] 에 / 가 들어있을 경우 PATH 탐색 없이 그 경로로 바로 검사

char	*cmd_path_center(t_mini *mini, char *cmd)
// return value: 올바른 실행 경로
{
	// int	i;
	char *path = NULL;
	// char	*whole;
	// 빌트인이면 널을 리턴한다
	// / 있는데 파일 없으면 127
	// / 있는데 access 못하면 126
	// / 있는데 위 2 경우 아니면 복사해서 리턴
	// / 없으면 path_array 없으면 127
	// / 없으면 쭉 붙여서 리렉토리인지 커미션 있는지 확인 

	(void)mini;
	(void)cmd;
	return path;
	// i = 0;
	// if (!mini || !cmd)
	// {
	// 	mini->exit_status = 0;
	// 	return (NULL);
	// }
	// if (im_bi(mini))
	// 	return (mini->exit_status = 0, NULL); // 빌트인 있고 파이프 없으면 패런트 실행 둘다 있으면 차일드 실행 처럼 엑섹으로 빼던가
	// if (ft_strchr(cmd, '/'))
	// // /이 없더라도 실행가능한지 확인해야 함
	// {
	// 	if (access(cmd, F_OK) != 0)
	// 	{
	// 		mini->exit_status = 127;
	// 		return (NULL);
	// 	}
	// 	if (access(cmd, X_OK) != 0)
	// 	{
	// 		mini->exit_status = 126;
	// 		return (NULL);
	// 	}
	// 	return (ft_strdup(cmd));
	// }
	// if (!mini->path_array)
	// 	return (mini->exit_status = 127, NULL);
	// i = 0;
	// while (mini->path_array[i])
	// {
	// 	path = ft_strjoin(mini->path_array[i], "/");
	// 	if (!path)
	// 		return (NULL);
	// 	whole = ft_strjoin(path, cmd);
	// 	free(path);
	// 	if (!whole)
	// 		return (NULL);
	// 	if (access(whole, X_OK) == 0)
	// 	{
	// 		if (im_directory(mini, whole))
	// 		{
	// 			mini->exit_status = 126;
	// 			free(whole);
	// 			return (NULL);
	// 		}
	// 		if (permission_ok(mini, whole) == 0)
	// 			return (whole);
	// 		free(whole);
	// 		return (mini->exit_status = 126, NULL);
	// 	}
	// 	free(whole);
	// 	i++;
	// }
	// return (mini->exit_status = 127, NULL);
}

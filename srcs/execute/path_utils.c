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
		fatal_error(mini, "PATH not found");
	mini->path_array = ft_split(env_path, ':');
	if (!mini->path_array)
		fatal_error(mini, "malloc error on PATH split");
	int i = 0;
	char* tmp;
	while (mini->path_array[i]) {
		tmp = mini->path_array[i];
		mini->path_array[i] = ft_strjoin(mini->path_array[i], "/");
		free(tmp);
		i++;
	}
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

char	*cmd_path_center(t_mini *mini, char *cmd)
{
	int		i;
	char	*whole;
	i = 0;

	if (!mini || !cmd)
	{
		mini->exit_status = 0;
		return (NULL);
	}
	// if (im_bi(mini))
	// 	return (mini->exit_status = 0, NULL); // 빌트인 있고 파이프 없으면 패런트 실행 둘다 있으면 차일드 실행 처럼 엑섹으로 빼던가
	
	if (does_file_exist(cmd)) {
		if (is_directory(cmd)) {
			// set exit_status as 126
			// return ?
		} else if (!is_executable(cmd)) {
			// set exit status as 126
			// return ?
		} else {
			return (ft_strdup(cmd));
		}
	}

	while (mini->path_array[i])
	{
		whole = ft_strjoin(mini->path_array[i], cmd);
		if (!whole)
			return (NULL);
		if (does_file_exist(whole)) {
			if (is_directory(cmd)) {
				// set exit_status as 126
				// return ?
			} else if (!is_executable(cmd)) {
				// set exit status as 126
				// return ?
			} else {
				return (whole);
			}
		}
		free(whole);
		i++;
	}
	
	return (mini->exit_status = 127, NULL);
}

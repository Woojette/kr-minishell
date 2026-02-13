#include "minishell.h"

//env 배열에서 path 환경변수 값 찾아 반환, "PATH=" 이후 문자열
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

// PATH 문자열 ':' 기준으로 분리해서 path_array 저장, 각 경로 뒤에 / 붙여서 붙이기 용이하게
void	set_path_array(t_mini *mini)
{
	char	*env_path;

	if (mini->path_array)
		return ;
	env_path = get_env_path(mini);
	//path 없으면 쉘 실행 불가
	if (!env_path)
		fatal_error(mini, "PATH not found");
	mini->path_array = ft_split(env_path, ':');
	if (!mini->path_array)
		fatal_error(mini, "malloc error on PATH split");
	int i = 0;
	char* tmp;
	//각 PATH 뒤에 / 추가
	while (mini->path_array[i]) 
	{
		tmp = mini->path_array[i];
		mini->path_array[i] = ft_strjoin(mini->path_array[i], "/");
		free(tmp);
		i++;
	}
}

//절대경로 찾는 함수 실행 가능한지 확인, 반환값 절대경로인데 말록한거 얘 부른애가 프리해야함 얘 안쓰나?
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
		tmp = ft_strjoin(mini->path_array[i], "/"); // 이거 문제 없는 거 같은데 헷갈리니까 체크하기 (위에 붙이는 거 관련!!!)
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

//절대경로나 상대경로 처리 함수, 파일 존재 실행 여부 확인, 이 순서는 적합한지 테스트 해서 확인함(어케했는지는 까먹음 근데 엑싯코드 똑같음)
char *get_path_absolute(t_mini *mini, char *cmd)
{
	if (does_file_exist(cmd)) 
	{
		if (is_directory(cmd)) 
		{
			mini->exit_status = 126;
			return NULL;
		} 
		else if (!is_executable(cmd)) 
		{
			mini->exit_status = 126;
			return NULL;
		} 
		else
			return (ft_strdup(cmd));
	}
	return (NULL);
}

// PATH 배열에서 cmd 실행 파일 찾기 PATH + cmd 조합으로 존재하는지 보고 폴더인지 실행가능한지 체크
char *get_path_envp(t_mini *mini, char *cmd)
{
	int	i;
	char	*whole;

	i = 0;
	while (mini->path_array[i])
	{
		whole = ft_strjoin(mini->path_array[i], cmd);
		if (!whole)
			return (NULL);
		if (does_file_exist(whole)) 
		{
			if (is_directory(whole) || !is_executable(whole)) 
			{
				mini->exit_status = 126;
				return (free(whole), NULL);
			}
			else 
				return (whole);
		}
		free(whole);
		i++;
	}
	return (mini->exit_status = 127, NULL); //커맨드 못찾으면 127 찍음~
}

//cmd 실행경로 최종적으로 결정, 1. 절대/상대 경로 먼저 확인 2.실패시 path 검색
char	*cmd_path_center(t_mini *mini, char *cmd)
{
	char	*found_path;
	if (!mini || !cmd || !cmd[0])//조건 확인하기
	{
		mini->exit_status = 0;
		return (NULL);
	}
	found_path = get_path_absolute(mini, cmd);
	if (found_path)	
		return (found_path);
	if (mini->exit_status == 126)
		return (NULL);
	return (get_path_envp(mini, cmd));
}

// char	*cmd_path_center(t_mini *mini, char *cmd)
// {
// 	int		i;
// 	char	*whole;
// 	i = 0;

// 	if (!mini || !cmd)
// 	{
// 		mini->exit_status = 0;
// 		return (NULL);
// 	}
// 	// if (im_bi(mini))
// 	// 	return (mini->exit_status = 0, NULL); // 빌트인 있고 파이프 없으면 패런트 실행 둘다 있으면 차일드 실행 처럼 엑섹으로 빼던가
	
// 	if (does_file_exist(cmd)) {
// 		if (is_directory(cmd)) {
// 		{	// set exit_status as 126
// 			return NULL;
// 		}
// 		} else if (!is_executable(cmd)) {
// 			// set exit status as 126
// 			return NULL;
// 		} else {
// 			return (ft_strdup(cmd));
// 		}
// 	}

// 	while (mini->path_array[i])
// 	{
// 		whole = ft_strjoin(mini->path_array[i], cmd);
// 		if (!whole)
// 			return (NULL);
// 		if (does_file_exist(whole)) {
// 			if (is_directory(whole)) {
// 				// set exit_status as 126
// 				return NULL;
// 				// return ?
// 			} else if (!is_executable(whole)) {
// 				// set exit status as 126
// 				return NULL;
// 			} else {
// 				return (whole);
// 			}
// 		}
// 		free(whole);
// 		i++;
// 	}
// 	return (mini->exit_status = 127, NULL);
// }

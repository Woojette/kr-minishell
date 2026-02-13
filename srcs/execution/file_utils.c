#include "minishell.h"

//stat 으로 파일 정보 가져옴 필로 타임함수처럼 구조체 존재하는 애
int	is_directory(char *file_path)
{
	struct stat buf;

	if (stat(file_path, &buf) == -1)
		return (0);
	return (S_ISDIR(buf.st_mode));
}

int	is_executable(char *file_path)
{
	struct stat buf;

	if (stat(file_path, &buf) == -1)
	{	
		return (0);
	}
	return (__S_IEXEC & buf.st_mode);
}
int	does_file_exist(char *file_path)
{
	struct stat buf;

	if (stat(file_path, &buf) == 0)
		return (1);
	return (0);
}

// fd 클로즈 후 fd 값 -1로 이닛, 중복 방지
void	ft_close(t_mini* mini, int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
		fatal_error(mini, "close");
	}
}

int	cmd_qqpart(t_mini *mini) // 이거 위치 바꿀거임 걍 유틸이라 넣어둔
{
	int	i;

	i = 0;
	if (!mini || !mini->cmd_array)
		return (0);
	while (i < mini->nbr_cmd)
	{
		if (mini->cmd_array[i].cmd && mini->cmd_array[i].cmd[0])
			return (1);
		i++;
	}
	return (0);
}
// file_utils.c
#include "exec_run.h"

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

void	ft_close(t_mini* mini, int *fd)
{
	if (fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
		fatal_error(mini, "close");
	}
}

#include "./mini_exec.h"

int	is_directory(char *cmd)
{
	struct stat	buf;

	stat(cmd, &buf);
	return (S_ISDIR(buf.st_mode));
}

int	have_permission(char *cmd)
{
	struct stat	buf;

	stat(cmd, &buf);
	return (S_IRWXU & buf.st_mode);
}

int	is_exist(char *cmd)
{
	struct stat	buf;
	int			result;

	result = stat(cmd, &buf);
	if (result == 0)
		return (1);
	return (0);
}

#include "minishell.h"

int	ft_pwd(void)
{
	char	buf[1024];
	size_t		size;

	size = sizeof(buf);
	if (getcwd(buf, size) != NULL)
	{
		printf("%s\n", buf);
		return (0);
	}
	else
	{
		printf("pwd: error retrieving current directory: ");
		printf("getcwd: cannot access parent directories: No such file or directory");
		printf("\n");
	}
	return (-1);
}

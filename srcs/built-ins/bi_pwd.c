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
// exit 관련 처리 적합해질 필요 있음
// 에러 메세지는 의문을 남김
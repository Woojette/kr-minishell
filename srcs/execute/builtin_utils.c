#include "exec_run.h"
#include "../built-ins/bi.h"

int	is_built_in(char *cmd)
{
	int	i;

	i = 0;
	// const char *bi_table[] = {"cd", "export", "unset", "exit", "echo", "pwd", "env", NULL};
	const char *bi_table[] = {"cd", NULL};
	

	while (bi_table[i])
	{
		if (!ft_strncmp(cmd, bi_table[i], ft_strlen(bi_table[i]) + 1))
			return (i + 1);
		i++;
	}
	return (0);
}

void	execute_built_in(t_mini *mini, char **cmd, int type)
{
	if (type == T_CD)
	{	
		ft_cd_all(cmd, mini->env);
	}
}
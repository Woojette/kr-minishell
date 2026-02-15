#include "minishell.h"

int	is_built_in(char *cmd)
{
	int	i;

	i = 0;
	// const char *bi_table[] = {"cd", "export", "unset", "exit", "echo", "pwd", "env", NULL};
	const char *bi_table[] = {"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL};

	
	if (!cmd || !*cmd)
		return (0);
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
		ft_cd_all(cmd, mini);
	}
	else if (type == T_ECHO)
	{
		ft_echo_all(cmd);
	}
	else if (type == T_PWD)
		ft_pwd();//오류 메세지 재확인 필요, 엑싯 스테이터스 외부나 내부 업뎃필요
	else if (type == T_EXPORT)
		ft_export_all(cmd, mini);
	else if (type == T_UNSET)
		ft_unset_all(cmd, mini);
	else if (type == T_ENV)
		ft_env(mini);
	else if (type == T_EXIT)
		ft_exit(cmd, mini);
}

void	execute_built_in2(t_mini *mini, char **cmd, int type)
{
	if (type == T_CD)
	{	
		ft_cd_all(cmd, mini);
	}
	else if (type == T_ECHO)
	{
		ft_echo_all(cmd);
	}
	else if (type == T_PWD)
		ft_pwd();//오류 메세지 재확인 필요, 엑싯 스테이터스 외부나 내부 업뎃필요
	else if (type == T_EXPORT)
		ft_export_all(cmd, mini);
	else if (type == T_UNSET)
		ft_unset_all(cmd, mini);
	else if (type == T_ENV)
		ft_env(mini);
	else if (type == T_EXIT)
		ft_exit2(cmd, mini);
}
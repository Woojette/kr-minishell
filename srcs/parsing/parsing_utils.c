// #include "../includes/minishell.h"
#include "../execute/exec_run.h"

int	ft_env_len(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		i ++;
	}
	return i;
}

void	copy_env(t_mini *mini, char **env)
{
	int	i;
	int	j;
	char **result;
	
	i = -1;
	if (!env)
		return ;
	result = malloc(sizeof(char*) * ft_env_len(env) + 1);
	if (!result)
		return ;
	while (env[++i])
	{
		result[i] = ft_strdup(env[i]);
		if (!result[i])
		{
			j = 0;
			while (j < i)
				free(result[j++]);
			return (free(result));
		}
	}
	result[i] = NULL;
	mini->env = result;
}

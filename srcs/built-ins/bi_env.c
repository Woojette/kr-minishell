#include "minishell.h"

void	ft_env(char **env)
{
	int	j;

	j = 0;
	while (env[j] != NULL)
	{
		printf("%s\n", env[j]);
		j++;
	}
}

int	ft_check_env_egal(char *str)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			break;
		i++;
	}
	if (str[i] == '\0')
		return (-1);
	return (i);
}

int	ft_check_env_double(char *str, char **env)
{
	int	i;
	int	j;
	int	egal;

	i = 0;
	j = 0;
	egal = ft_check_env_egal(str);
	if (egal == -1)
		return (-1);
	while (env[j] != NULL)
	{
		if (ft_strncmp(str, env[j], egal + 1) == 0)
			return (j);
		j++;
	}
	return (0);
}

#include "exec_run.h"

void	free_path(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
void	cleanup_paths(t_mini *mini)
{
	if (mini->path_array)
	{
		free_path(mini->path_array);
		mini->path_array = NULL;
	}
}

void	cleanup_all(t_mini *mini)
{
	int	i;

	i = 0;
	if (!mini)
		return ;
	while (i < mini->nbr_cmd)
	{
		free_path(mini->cmd_array[i].cmd);
		mini->cmd_array[i].cmd = NULL;
		if (mini->cmd_array[i].fd_in >= 0)
			ft_close(&mini->cmd_array[i].fd_in);
		if (mini->cmd_array[i].fd_out >= 0)
			ft_close(&mini->cmd_array[i].fd_out);
		pipex_free(mini, i);
		i++;
	}
	cleanup_paths(mini);
}
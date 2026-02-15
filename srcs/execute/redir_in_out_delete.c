#include "minishell.h"

int	before_inout(t_mini *mini, int i)
{
	if (!mini || i < 0 || i >= mini->nbr_cmd)
		return (-1);
	if (!mini->cmd_array || !mini->cmd_array[i].inoutfile)
		return (-1);
	if (mini->cmd_array[i].inout_fail)
		return (-1);
	return (0);
}

void	inout_redir(t_mini *mini, int i)
{
	int	n;
	int	k;
	int	type_redir;
	int	fd_temp;

	n = 0;
	k = 0;
	type_redir = DEFAULT;
	fd_temp = -1;
	if (before_inout(mini, i) < 0)
		return ; 
	while (mini->cmd_array[i].inoutfile[n])
	{
		type_redir = DEFAULT;
		fd_temp = -1;
		if (!mini->cmd_array[i].ihoa
			|| n >= len_tab_char(mini->cmd_array[i].inoutfile))
			type_redir = DEFAULT;
		else
			type_redir = mini->cmd_array[i].ihoa[n];
		if (type_redir == TRUNC)
		{
			fd_temp = open(mini->cmd_array[i].inoutfile[n],
					O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].inoutfile[n]);
				mini->exit_status = 1;
				mini->cmd_array[i].inout_fail = 1;
				if (mini->cmd_array[i].fd_out != -1)
				{
					ft_close(mini->cmd_array[i].fd_out);
					mini->cmd_array[i].fd_out = -1;
				}
				return ;
			}
		}
		else if (type_redir == APPEND)
		{
			fd_temp = open(mini->cmd_array[i].inoutfile[n],
					O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].inoutfile[n]);
				mini->exit_status = 1;
				mini->cmd_array[i].inout_fail = 1;
				if (mini->cmd_array[i].fd_out != -1)
				{
					ft_close(mini->cmd_array[i].fd_out);
					mini->cmd_array[i].fd_out = -1;
				}
				return ;
			}
		}
		else if (type_redir == INF)
		{
			fd_temp = open(mini->cmd_array[i].inoutfile[n], O_RDONLY);
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].inoutfile[n]);
				mini->exit_status = 1;
				mini->cmd_array[i].inout_fail = 1;
				if (mini->cmd_array[i].fd_in != -1)
				{
					ft_close(mini->cmd_array[i].fd_in);
					mini->cmd_array[i].fd_in = -1;
				}
				return ;
			}
		}
		else if (type_redir == HEREDOC)
		{
			if (!mini->cmd_array[i].temp_heredoc
				|| !mini->cmd_array[i].temp_heredoc[k])
			{
				mini->exit_status = 1;
				mini->cmd_array[i].inout_fail = 1;
				if (mini->cmd_array[i].fd_in != -1)
				{
					ft_close(mini->cmd_array[i].fd_in);
					mini->cmd_array[i].fd_in = -1;
				}
				return ;
			}
			fd_temp = open(mini->cmd_array[i].temp_heredoc[k], O_RDONLY);
			if (fd_temp < 0)
			{
				perror(mini->cmd_array[i].temp_heredoc[k]);
				mini->exit_status = 1;
				mini->cmd_array[i].inout_fail = 1;
				if (mini->cmd_array[i].fd_in != -1)
				{
					ft_close(mini->cmd_array[i].fd_in);
					mini->cmd_array[i].fd_in = -1;
					mini->cmd_array[i].temp_heredoc[k] = NULL;
					init_signaux();
				}
				return ;
			}
			k++;
		}
		if ((type_redir == INF || type_redir == HEREDOC)
			&& (mini->cmd_array[i].fd_in != -1))
		{
			ft_close(mini->cmd_array[i].fd_in);
			mini->cmd_array[i].fd_in = -1;
		}
		if ((type_redir == APPEND || type_redir == TRUNC)
			&& mini->cmd_array[i].fd_out != -1)
		{
			ft_close(mini->cmd_array[i].fd_out);
			mini->cmd_array[i].fd_out = -1;
		}
		if (type_redir == TRUNC || type_redir == APPEND)
			mini->cmd_array[i].fd_out = fd_temp;
		else if (type_redir == INF || type_redir == HEREDOC)
			mini->cmd_array[i].fd_in = fd_temp;
		if (mini->cmd_array[i].inout_fail != 1)
			mini->exit_status = 0;
		n++;
	}
}


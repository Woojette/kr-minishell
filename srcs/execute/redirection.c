#include "exec_run.h"

int	redirection_center(t_mini *mini)
{
	int	i;
	i = 0;

	if (!mini || !mini->cmd_array || mini->nbr_cmd <= 0)
		return (-1);
	while (i < mini->nbr_cmd)
	{
		t_cmd *c = &mini->cmd_array[i];
		if (!c->in_fail && c->compter_heredoc > 0)
		{
			if (appliquer_heredoc_cmd(mini, i) == -1)
			{
				c->in_fail = 1;
			}
		}
		if (!c->in_fail)
			appliquer_in_redirs_en_ordre(mini, c);
		if (!c->out_fail && c->outfile && c->outfile[0])
			process_out_redir(mini, i);
		i++;
	}
}
int	appliquer_infile_one(t_mini *mini, t_cmd *c, char *pathcheck)
{
	int	fd;

	if (!pathcheck || !*pathcheck)
	{
		c->in_fail = 1;
		mini->exit_status = 1;
		return (-1);
	}
	fd = open(pathcheck, O_RDONLY);
	if (fd < 0)
	{
		perror(pathcheck);
		c->in_fail = 1;
		mini->exit_status = 1;
		return (-1);
	}
	ft_close(&c->fd_in);
	c->fd_in = fd;
	return (0);
}

int	appliquer_heredoc_one(t_mini *mini, t_cmd *c, int i)
{
	int fd;
	
	if (!mini || !c || !c->temp_heredoc || i < 0 || !c->temp_heredoc[i])
	{
		c->in_fail = 1;
		mini->exit_status = 1;
		return (-1);
	}
	// tmp_path = c->temp_heredoc[i];
	fd = open(c->temp_heredoc[i], O_RDONLY);
	if (fd < 0)
	{
		perror(c->temp_heredoc[i]);
		c->in_fail = 1;
		mini->exit_status = 1;
		return (-1);
	}
	ft_close(&c->fd_in);
	c->fd_in = fd;
	return (0);
}

int	cut_appliquer_in_redirs_en_ordre(t_mini *mini, t_cmd *c)
{
	c->in_fail = 1;
	mini->exit_status = 1;
	init_signaux();
	return (-1);
}

int	divide_appl_redirs(t_mini *mini, t_cmd *c, int kind, int i)
{
	if (kind == 0)
	{
		if (!c->infile || i < 0 || !c->infile[i])
			return (cut_appliquer_in_redirs_en_ordre(mini, c));
		return (appliquer_infile_one(mini, c, c->infile[i]));
	}
	else if (kind == 1)
		return(appliquer_heredoc_one(mini, c, i));
	else
		return (cut_appliquer_in_redirs_en_ordre(mini, c));	
}

int	appliquer_in_redirs_en_ordre(t_mini *mini, t_cmd *c)
{
	int	j;

	if (!mini || !c)
		return (-1);
	if (c->compter_in_hd <= 0 || !c->in_heredoc || !c->in_hd_index)
		return (0);
	j = 0;
	while (j < c->compter_in_hd)
	{
		if (divide_appl_redirs(mini, c, c->in_heredoc[j], c->in_hd_index[j]) < 0)
			return (-1);
		if (c->in_fail)
			return (-1);
		j++;
	}
	return (0);
}

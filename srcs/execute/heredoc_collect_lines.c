/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect_lines.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 12:32:59 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/18 17:39:34 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*util_hd_line(t_cmd *cmd, t_mini *mini, int n, char *line)
{
	if (!(cmd->hd_env && cmd->hd_env[n]))
		return (line);
	return (remplacer_dollar(line, mini));
}

int	write_line(int fd, char *str)
{
	if (write(fd, str, ft_strlen(str)) == -1)
		return (-1);
	if (write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

int	cut_while_c_l(void)
{
	if (g_exit_status == 130)
		return (-2);
	return (1);
}

int	while_collect_lines(int fd, t_mini *mini, int j, int n)
{
	char	*line;
	char	*line_applique;
	t_cmd	*cmd;

	cmd = &mini->cmd_array[j];
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (cut_while_c_l());
		if (cmd->limiter[n] && ft_strcmp(line, cmd->limiter[n]) == 0)
			return (free(line), 0);
		line_applique = util_hd_line(cmd, mini, n, line);
		if (!line_applique)
			return (free(line), -1);
		if (write_line(fd, line_applique) == -1)
		{
			if (line_applique != line)
				free(line_applique);
			return (free(line), -1);
		}
		if (line_applique != line)
			free(line_applique);
		free(line);
	}
}

int	collecter_heredoc_lines(int fd, t_mini *mini, int j, int n)
{
	if (fd < 0 || !mini || !mini->cmd_array || j < 0 || j >= mini->nbr_cmd
		|| !mini->cmd_array[j].limiter || n < 0
		|| n >= mini->cmd_array[j].compter_heredoc)
		return (-1);
	if (!mini->cmd_array[j].limiter[n])
		return (1);
	return (while_collect_lines(fd, mini, j, n));
}

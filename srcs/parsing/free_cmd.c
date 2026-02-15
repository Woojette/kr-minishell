/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/14 17:58:04 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_tab_char(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_tab_char(cmd->cmd);
	cmd->cmd = NULL;
	free_tab_char(cmd->inoutfile);
	cmd->inoutfile = NULL;
	free_temp_heredoc(cmd->temp_heredoc);
	cmd->temp_heredoc = NULL;
	free_tab_char(cmd->limiter);
	cmd->limiter = NULL;
}

void	free_cmd_fd_tab_sans_hd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->fd_in != -1)
		close(cmd->fd_in);
	cmd->fd_in = -1;
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
	cmd->fd_out = -1;
	free_cmd_tab_char(cmd);
	free_tab_int(cmd->ihoa);
	cmd->ihoa = NULL;
	free_tab_int(cmd->in_heredoc);
	cmd->in_heredoc = NULL;
	free_tab_int(cmd->hd_env);
	cmd->hd_env = NULL;
}

void	free_cmd_partiel(t_cmd *cmd, int nbr_cmd)
{
	int	i;

	if (!cmd || nbr_cmd <= 0)
		return ;
	i = 0;
	while (i < nbr_cmd)
	{
		free_cmd_tab_char(&cmd[i]);
		i++;
	}
}

void	free_cmd_interieur(t_cmd *cmd, int nbr_cmd)
{
	int	i;

	if (!cmd || nbr_cmd <= 0)
		return ;
	i = 0;
	while (i < nbr_cmd)
	{
		free_cmd_fd_tab_sans_hd(&cmd[i]);
		i++;
	}
}

void	free_cmd_all(t_cmd *cmd, int nbr_cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (i < nbr_cmd)
	{
		free_cmd_fd_tab_sans_hd(&cmd[i]);
		i++;
	}
	free(cmd);
}

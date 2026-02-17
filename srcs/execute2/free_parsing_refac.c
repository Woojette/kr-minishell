/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing_refac.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:43:23 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 19:53:45 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_fd_tab(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->fd_in != -1)
		close(cmd->fd_in);
	cmd->fd_in = -1;
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
	cmd->fd_out = -1;
	free_tab_char(cmd->cmd);
	cmd->cmd = NULL;
	free_tab_char(cmd->inoutfile);
	cmd->inoutfile = NULL;
	free_temp_heredoc(cmd->temp_heredoc);
	cmd->temp_heredoc = NULL;
	free_tab_char(cmd->limiter);
	cmd->limiter = NULL;
	free_tab_int(cmd->ihoa);
	cmd->ihoa = NULL;
	free_tab_int(cmd->in_heredoc);
	cmd->in_heredoc = NULL;
	free_tab_int(cmd->hd_env);
	cmd->hd_env = NULL;
}

void	free_round(t_mini *mini)
{
	if (!mini)
		return ;
	if (mini->cmd_array)
	{
		free_cmd_all(mini->cmd_array, mini->nbr_cmd);
		mini->cmd_array = NULL;
		mini->nbr_cmd = 0;
	}
	if (mini->path_array)
		cleanup_paths(mini);
	if (mini->pipe_read_end >= 0)
		close(mini->pipe_read_end);
	mini->pipe_read_end = -1;
}

void	free_mini(t_mini *mini)
{
	if (!mini)
		return ;
	if (mini->cmd_array)
		free_cmd_all(mini->cmd_array, mini->nbr_cmd);
	mini->cmd_array = NULL;
	mini->nbr_cmd = 0;
	if (mini->path_array)
		ft_free_tab(mini->path_array);
	mini->path_array = NULL;
	if (mini->env)
		free_tab_char(mini->env);
	mini->env = NULL;
	if (mini->save_ex)
		free_tab_char(mini->save_ex);
	mini->save_ex = NULL;
	if (mini->pipe_read_end >= 0)
		close(mini->pipe_read_end);
	if (mini->m_ptr)
		free(mini->m_ptr);
	mini->m_ptr = NULL;
	free(mini);
}

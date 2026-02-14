/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
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
	// free_tab_char(cmd->outfile);
	// cmd->outfile = NULL;
	free_temp_heredoc(cmd->temp_heredoc); // supprimer tous les fichiers temporaires de heredoc (unlink), puis free le tableau temp_heredoc[]
	cmd->temp_heredoc = NULL;
	free_tab_char(cmd->limiter);
	cmd->limiter = NULL;
}

// free les fd de cmd, puis free les tableaux infile[], outfile[], temp_heredoc[], limiter[], out_append[], in_heredoc[], in_hd_index[] de cmd
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
	free_cmd_tab_char(cmd); // free les tableaux de string (cmd, infile, outfile, temp_heredoc, limiter)
	free_tab_int(cmd->ihoa);
	cmd->ihoa = NULL;
	free_tab_int(cmd->in_heredoc);
	cmd->in_heredoc = NULL;
	free_tab_int(cmd->hd_env);
	cmd->hd_env = NULL;
}

// free uniquement les tableaux de chaine (char **) dans cmd[0 ~ nbr_cmd-1], pas les fd et les tableaux d'int
// free oui: cmd[i].cmd, cmd[i].inoutfile, cmd[i].temp_heredoc, cmd[i].limiter
// free non: fd_in, fd_out, cmd[i].ihoa, cmd[i].in_heredoc, cmd[i].hd_env
void  free_cmd_partiel(t_cmd *cmd, int nbr_cmd)
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

// free le contenu de chaque structure cmd (fd, tableaux de string et d'int), mais pas le tableau de cmd lui-meme
// pour un nettoyage partiel apres une erreur pendant le parsing
void  free_cmd_interieur(t_cmd *cmd, int nbr_cmd)
{
  int	i;

  if (!cmd || nbr_cmd <= 0)
    return ;
  i = 0;
  while (i < nbr_cmd)
  {
    free_cmd_fd_tab(&cmd[i]); // free fd + char ** + int *
    i++;
  }
}

// free tous les cmd (fd, tableaux de string et d'int), puis free le tableau de cmd
void	free_cmd_all(t_cmd *cmd, int nbr_cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (i < nbr_cmd) // pour chaque cmd, free les fd et les tableaux de string et d'int
	{
		free_cmd_fd_tab(&cmd[i]); // free les fd et les tableaux de string et d'int de cmd[i]
		i++;
	}
	free(cmd); // free le tableau de cmd apres avoir free tous les cmd
}

#include "minishell.h"

// liste free
void	free_tokens(t_token **token)
{
	t_token	*tmp;

	while (*token)
	{
		tmp = *token;
		*token = (*token)->next;
		free(tmp->str); // free la string du token
		free(tmp); // free le token lui-meme
	}
	*token = NULL; // reinitialiser a NULL pour eviter les dangling pointer
}

// free le tableau de string (ex. infile[], outfile[], temp_heredoc[], limiter[])
void	free_tab_char(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

// free le tableau d'int (ex. out_append[], in_heredoc[], in_hd_index[], hd_env[])
void	free_tab_int(int *tab)
{
	if (!tab)
		return ;
	free(tab);
}

// supprimer tous les fichiers temporaires de heredoc (unlink), puis free le tableau temp_heredoc[]
void	free_temp_heredoc(char **temp)
{
	int	i;

	if (!temp)
		return ;
	i = 0;
	while (temp[i])
	{
		unlink(temp[i]); // supprimer le fichier temporaire du heredoc
		free(temp[i]); // free le nom du fichier temporaire du heredoc
		i++;
	}
	free(temp); // free le tableau de string (char **) apres avoir supprimer les fichiers temporaires
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
	free_tab_char(cmd->cmd);
	cmd->cmd = NULL;
	free_tab_char(cmd->infile);
	cmd->infile = NULL;
	free_tab_char(cmd->outfile);
	cmd->outfile = NULL;
	free_temp_heredoc(cmd->temp_heredoc); // supprimer tous les fichiers temporaires de heredoc (unlink), puis free le tableau temp_heredoc[]
	cmd->temp_heredoc = NULL;
	free_tab_char(cmd->limiter);
	cmd->limiter = NULL;
	free_tab_int(cmd->out_append);
	cmd->out_append = NULL;
	free_tab_int(cmd->in_heredoc);
	cmd->in_heredoc = NULL;
	free_tab_int(cmd->in_hd_index);
	cmd->in_hd_index = NULL;
	free_tab_int(cmd->hd_env);
	cmd->hd_env = NULL;
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

// free la structure mini apres avoir free tous les cmd
// env: on utilise directement le env de main, donc on ne le free pas (c'est le env de main, pas une copie)
void	free_mini(t_mini *mini)
{
	if (!mini)
		return ;
	if (mini->cmd)
		free_cmd_all(mini->cmd, mini->nbr_cmd); // free tous les cmd (fd, tableaux de string et d'int), puis free le tableau de cmd
	free(mini); // free la structure mini apres avoir free tous les cmd
}

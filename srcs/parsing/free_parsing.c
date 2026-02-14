/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// free la structure mini apres avoir free tous les cmd
// env: on utilise directement le env de main, donc on ne le free pas (c'est le env de main, pas une copie)
void	free_mini(t_mini *mini)
{
	if (!mini)
		return ;
	if (mini->cmd_array)
		free_cmd_all(mini->cmd_array, mini->nbr_cmd); // free tous les cmd (fd, tableaux de string et d'int), puis free le tableau de cmd
	free_tab_char(mini->path_array); // free le tableau de string pour les chemins d'acces des commandes
	mini->path_array = NULL;
	free(mini); // free la structure mini apres avoir free tous les cmd
}

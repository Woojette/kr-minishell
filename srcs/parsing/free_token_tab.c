/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

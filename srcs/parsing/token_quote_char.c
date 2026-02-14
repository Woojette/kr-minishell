/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quote_char.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// recuprer le caractere de la premiere quote
char	caractere_quote_debut(char *line)
{
	int		i;
	char	debut_quote;

	i = 0;
	debut_quote = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			debut_quote = line[i];
			break ;
		}
		i++;
	}
	return (debut_quote);
}

// recuperer l'index de la premiere quote 
int	index_quote_debut(char *line, char c)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == c)
			break ;
		i++;
	}
	return (i);
}

// recuperer l'index de la deuxieme quote 
int	index_quote_fin(char *line, char c)
{
	int	i;
	int	debut_quote;

	debut_quote = index_quote_debut(line, c);
	i = debut_quote + 1;
	while (line[i])
	{
		if (line[i] == c)
			break ;
		i++;
	}
	return (i);
}

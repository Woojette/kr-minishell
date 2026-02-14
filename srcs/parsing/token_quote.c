/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	if (!line[i])
		return (0);
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_avant_quote_espace(char *line)
{
	int	i;
	int	pos_quote;

	if (!line || !*line)
		return (0);
	i = 0;
	while (line[i] && line[i] != '"' && line[i] != '\'')
		i++;
	if (!line[i])
		return (0);
	pos_quote = i;
	if (pos_quote == 0)
		return (1);
	i = pos_quote - 1;
	while (i >= 0)
	{
		if (line[i] == ' ' || line[i] == '>' || line[i] == '<'
			|| line[i] == '|')
			return (1);
		i--;
	}
	return (0);
}

int	check_2_quotes_milieu_puis_fin(char *line)
{
	int		i;
	char	debut_quote;

	debut_quote = caractere_quote_debut(line);
	if (debut_quote == 0 || !check_quote_milieu_ok(line))
		return (-1);
	i = index_quote_debut(line, debut_quote);
	if (i < 0)
		return (-1);
	i++;
	while (line[i])
	{
		if (line[i] == debut_quote)
		{
			i++;
			if (line[i] == ' ' || line[i] == '\0' || line[i] == '>'
				|| line[i] == '<' || line[i] == '|')
				return (1);
			return (0);
		}
		i++;
	}
	return (0);
}

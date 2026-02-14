/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quote_position.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote_debut_ok(char *line)
{
	char	debut_quote;
	int		compter_quote;
	int		i;

	if (!line || (line[0] != '"' && line[0] != '\''))
		return (0);
	debut_quote = line[0];
	if (line[0] == '"' || line[0] == '\'')
		compter_quote = 1;
	else
		return (0);
	i = 1;
	while (line[i])
	{
		if (line[i] == debut_quote)
		{
			compter_quote++;
			break ;
		}
		i++;
	}
	if (compter_quote == 2)
		return (1);
	return (0);
}

int	check_2_quotes_debut_puis_fin(char *line)
{
	int		i;
	char	quote;

	quote = line[0];
	i = 1;
	if (check_quote_debut_ok(line) == 1)
	{
		while (line[i])
		{
			if (line[i] == quote)
				break ;
			i++;
		}
		i++;
		if (line[i] == ' ' || line[i] == '\0' || line[i] == '>'
			|| line[i] == '<' || line[i] == '|')
			return (1);
	}
	return (0);
}

int	check_quote_fermee(char *line, int debut_quote, int compte_debut_quote)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == debut_quote)
			compte_debut_quote++;
		if (compte_debut_quote == 2)
			return (1);
		i++;
	}
	return (0);
}

int	check_quote_milieu_ok(char *line)
{
	char	debut_quote;
	int		compte_debut_quote;
	int		i;

	debut_quote = 0;
	compte_debut_quote = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			debut_quote = line[i];
			compte_debut_quote++;
			break ;
		}
		i++;
	}
	if (debut_quote == 0)
		return (0);
	if (check_quote_fermee(line, debut_quote, compte_debut_quote) == 1)
		return (1);
	return (0);
}

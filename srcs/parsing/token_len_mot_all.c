/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_len_mot_all.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_quote_premier(char *line, int *len)
{
	int	debut_quote;
	int	quote_fermee;

	debut_quote = check_quote_debut_ok(line);
	quote_fermee = check_2_quotes_debut_puis_fin(line);
	if (debut_quote == 1 && quote_fermee == 1)
		*len = len_mot_2_quotes_entier(line);
	else if (debut_quote == 1 && quote_fermee == 0)
		*len = len_mot_2_quotes_entier(line) + len_mot_apres_quote(line);
	else if (debut_quote == 0)
		*len = len_mot_sans_quote(line);
	else
		return (0);
	return (1);
}

int	appliquer_quote_milieu(char *line, int *len)
{
	int	milieu_quote;
	int	espace_avant_quote;
	int	quote_fermee;

	milieu_quote = check_quote_milieu_ok(line);
	espace_avant_quote = check_avant_quote_espace(line);
	quote_fermee = check_2_quotes_milieu_puis_fin(line);
	if (milieu_quote == 1 && espace_avant_quote == 0 && quote_fermee == 1)
		*len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line);
	else if (milieu_quote == 1 && espace_avant_quote == 0 && quote_fermee == 0)
		*len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line)
			+ len_mot_apres_quote(line);
	else if (milieu_quote == 0 && espace_avant_quote == 0)
		*len = len_mot_sans_quote(line);
	else
		*len = len_mot_sans_quote(line);
	return (1);
}

int	len_mot_total(char *line)
{
	int	len;

	len = 0;
	if (!line || line[0] == ' ' || line[0] == '\t' || line[0] == '>'
		|| line[0] == '<' || line[0] == '|')
		return (0);
	if (line[0] == '"' || line[0] == '\'')
	{
		if (appliquer_quote_premier(line, &len) == 1)
			return (len);
		return (len_mot_sans_quote(line));
	}
	else if (line[0] != '"' && line[0] != '\'')
	{
		if (appliquer_quote_milieu(line, &len) == 1)
			return (len);
	}
	return (len);
}

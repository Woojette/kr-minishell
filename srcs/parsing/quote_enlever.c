/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_enlever.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	inverser_et_incrementer(int *quote, int *i)
{
	*quote = !(*quote);
	(*i)++;
}

void	init_enlever_quote(t_quote *quote)
{
	quote->i = 0;
	quote->n = 0;
	quote->s_quote = 0;
	quote->d_quote = 0;
}

void	quote_a_enlever(char *str, char *resultat, t_quote *quote)
{
	if (str[quote->i] == '\'' && !quote->d_quote)
		inverser_et_incrementer(&quote->s_quote, &quote->i);
	else if (str[quote->i] == '"' && !quote->s_quote)
		inverser_et_incrementer(&quote->d_quote, &quote->i);
	else
		resultat[quote->n++] = str[quote->i++];
}

char	*enlever_quote_dans_token(char *str)
{
	t_quote	quote;
	char	*resultat;

	if (!str)
		return (NULL);
	init_enlever_quote(&quote);
	resultat = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!resultat)
		return (NULL);
	while (str[quote.i])
		quote_a_enlever(str, resultat, &quote);
	resultat[quote.n] = '\0';
	if (quote.s_quote || quote.d_quote)
		return (free(resultat), NULL);
	return (resultat);
}

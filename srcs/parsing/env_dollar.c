/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_dollar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_val(int *i, int *s_quote, int *d_quote, char **resultat)
{
	*i = 0;
	*s_quote = 0;
	*d_quote = 0;
	(*resultat) = malloc(sizeof(char) * 1);
	if (!(*resultat))
		return ;
	(*resultat)[0] = '\0';
}

char	*char_et_quote(char *resultat, int *i, int *quote, char *str)
{
	(*quote) = !(*quote);
	resultat = ajouter_char(resultat, str[(*i)++]);
	return (resultat);
}

char	*remplacer_dollar(char *str, t_mini *mini)
{
	int		i;
	int		s_quote;
	int		d_quote;
	char	*resultat;

	if (!str || !mini)
		return (NULL);
	init_val(&i, &s_quote, &d_quote, &resultat);
	if (!resultat)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote)
			resultat = char_et_quote(resultat, &i, &s_quote, str);
		else if (str[i] == '"' && !s_quote)
			resultat = char_et_quote(resultat, &i, &d_quote, str);
		else if ((str[i] == '$' && str[i + 1] != '\0') && !s_quote)
			resultat = appliquer_env_var(resultat, str, mini, &i);
		else
			resultat = ajouter_char(resultat, str[i++]);
		if (!resultat)
			return (NULL);
	}
	return (resultat);
}

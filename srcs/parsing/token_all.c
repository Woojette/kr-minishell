/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_all.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	traiter_redirection(char **line, t_token **token, t_type_token *fd_type)
{
	int	resultat;

	resultat = appliquer_redir_token(line, token, fd_type);
	if (resultat == -1)
		return (-1);
	return (1); // continue
}

int	traiter_pipe(char **line, t_token **token, t_type_token *fd_type)
{
	int	resultat;

	resultat = appliquer_pipe_token(line, token, fd_type);
	if (resultat < 0)
		return (resultat);
	return (1); // continue
}

int	traiter_mot(char **line, t_token **token, t_type_token *fd_type, int *len)
{
	if (appliquer_mot_token(line, token, fd_type, len) < 0)
		return (-1);
	return (0); // ok, continuer normalement
}

int	traiter_token(char **line, t_token **token, t_type_token *fd_type, int *len)
{
	int	resultat;

	if (est_espace(**line))
	{
		passer_espace(line);
		return (1); // continue
	}
	else if (est_redirection(*line))
	{
		resultat = traiter_redirection(line, token, fd_type);
		if (resultat <= 0)
			return (resultat);
		return (1); // continue
	}
	else if (est_pipe(*line))
	{
		resultat = traiter_pipe(line, token, fd_type);
		if (resultat < 0)
			return (resultat);
		return (1); // continue
	}
	else
		return (traiter_mot(line, token, fd_type, len));
}

// On parse tout pour trouver les operations ou les builtins
// chaque noeud serait d'abord divise que par soit mot, soit redir, soit pipe  (cf. t_type token)
int	parse_input(char *line, t_token **token, t_mini *mini)
{
	int				len;
	int				resultat;
	t_type_token	fd_type;

	if (!line || !token || !mini)
		return (-1);
	len = 0;
	fd_type = (t_type_token) - 1;
	while (*line)
	{
		resultat = traiter_token(&line, token, &fd_type, &len);
		if (resultat < 0)
			return (resultat);
	}
	return (appliquer_token_final(token, fd_type, mini));
}

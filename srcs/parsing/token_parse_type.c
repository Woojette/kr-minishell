/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse_type.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	passer_espace(char **line)
{
	while ((**line) == ' ' || (**line) == '\t')
		(*line)++;
	return ;
}

int	appliquer_pipe_token(char **line, t_token **token, t_type_token *fd_type)
{
	if ((*fd_type) != (t_type_token)-1)
		return (free_tokens(token), write(2,
				"syntax error near unexpected token `|'\n", 40), -2);
	if (add_token((*line), T_PIPE, 1, token) < 0)
		return (free_tokens(token), -1);
	(*line) += 1;
	return (0);
}

int	appliquer_mot_token(char **line, t_token **token, t_type_token *fd_type,
		int *len)
{
	(*len) = len_mot_total((*line));
	if ((*len) <= 0)
		return (free_tokens(token), -1);
	if ((*fd_type) != (t_type_token)-1)
	{
		if (add_token((*line), (*fd_type), (*len), token) < 0)
			return (free_tokens(token), -1);
		(*fd_type) = (t_type_token) - 1;
	}
	else
	{
		if (add_token((*line), T_MOT, (*len), token) < 0)
			return (free_tokens(token), -1);
	}
	(*line) += (*len);
	return (0);
}

int	appliquer_token_final(t_token **token, t_type_token fd_type, t_mini *mini)
{
	if (fd_type != (t_type_token)-1)
		return (free_tokens(token), write(2,
				"syntax error near unexpected token `newline'\n", 45), -2);
	if (appliquer_dollar_sur_liste_token(token, mini) == -1)
		return (free_tokens(token), -1);
	if (appliquer_quote_sur_liste_token(token) == -1)
		return (free_tokens(token), -1);
	return (0);
}

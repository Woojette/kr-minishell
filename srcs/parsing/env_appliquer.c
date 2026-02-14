/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_appliquer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_dollar_sur_liste_token(t_token **token, t_mini *mini)
{
	char	*new_str;
	t_token	*temp;

	if (!token || !(*token) || !mini)
		return (-1);
	temp = *token;
	while (temp)
	{
		if (temp->type_token == T_MOT || temp->type_token == T_FD_IN
			|| temp->type_token == T_FD_OUT
			|| temp->type_token == T_FD_OUT_APPEND)
		{
			if (!temp->str)
				return (-1);
			new_str = remplacer_dollar(temp->str, mini);
			if (!new_str)
				return (-1);
			free(temp->str);
			temp->str = new_str;
		}
		temp = temp->next;
	}
	return (0);
}

int	appliquer_quote_sur_liste_token(t_token **token)
{
	char	*new_str;
	t_token	*temp;

	if (!token || !(*token))
		return (-1);
	temp = *token;
	while (temp)
	{
		if (temp->str && (temp->type_token == T_MOT
				|| temp->type_token == T_FD_IN || temp->type_token == T_FD_OUT
				|| temp->type_token == T_FD_OUT_APPEND
				|| temp->type_token == T_FD_HEREDOC))
		{
			new_str = enlever_quote_dans_token(temp->str);
			if (!new_str)
				return (-1);
			free(temp->str);
			temp->str = new_str;
		}
		temp = temp->next;
	}
	return (0);
}

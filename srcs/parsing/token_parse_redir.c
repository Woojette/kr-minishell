/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_redir_2_len(char **line, t_token **token, t_type_token *fd_type)
{
	if (!ft_strncmp((*line), ">>", 2))
	{
		if (add_token((*line), T_RD_APPEND, 2, token) < 0)
			return (free_tokens(token), -1);
		(*fd_type) = T_FD_OUT_APPEND;
		return ((*line) += 2, 1);
	}
	else if (!ft_strncmp((*line), "<<", 2))
	{
		if (add_token((*line), T_RD_HEREDOC, 2, token) < 0)
			return (free_tokens(token), -1);
		(*fd_type) = T_FD_HEREDOC;
		return ((*line) += 2, 1);
	}
	return (0);
}

int	appliquer_redir_1_len(char **line, t_token **token, t_type_token *fd_type)
{
	if (!ft_strncmp((*line), ">", 1))
	{
		if (add_token((*line), T_RD_OUT, 1, token) < 0)
			return (free_tokens(token), -1);
		(*fd_type) = T_FD_OUT;
		return ((*line) += 1, 1);
	}
	else if (!ft_strncmp((*line), "<", 1))
	{
		if (add_token((*line), T_RD_IN, 1, token) < 0)
			return (free_tokens(token), -1);
		(*fd_type) = T_FD_IN;
		return ((*line) += 1, 1);
	}
	return (0);
}

int	appliquer_redir_token(char **line, t_token **token, t_type_token *fd_type)
{
	if (!ft_strncmp((*line), ">>", 2) || !ft_strncmp((*line), "<<", 2))
		return (appliquer_redir_2_len(line, token, fd_type));
	else if (!ft_strncmp((*line), ">", 1) || !ft_strncmp((*line), "<", 1))
		return (appliquer_redir_1_len(line, token, fd_type));
	return (0);
}

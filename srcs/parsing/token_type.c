/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	new_node_init(t_token **new_node, char *line, int len, t_type_token type_token)
{
	(*new_node)->str = ft_strndup(line, len);
	if (!(*new_node)->str)
	{
		free((*new_node));
		return (-1);
	}
	(*new_node)->type_token = type_token;
	(*new_node)->type_quote = GENERAL;
	(*new_node)->type_bi = 0; // par defaut
	(*new_node)->next = NULL;
	return (0);
}

// On ajoute le token dans la structure t_token;
int add_token(char *line, t_type_token type_token, int len, t_token **token)
{
	t_token *new_node;
	t_token *tmp;

	if (!line || !token || len <= 0)
		return (-1);
	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (-1);
	if (new_node_init(&new_node, line, len, type_token) == -1)
		return (-1);
	if (*token == NULL) // Si la liste est vide on met au debut
			*token = new_node;
	else
	{
			tmp = *token;
			while (tmp->next)
					tmp = tmp->next;
			tmp->next = new_node;
	}
	return (0);
}

void	appliquer_token_fd(t_token *temp)
{
	if (!temp)
		return ;
	if (temp->type_token == T_RD_IN)
	{
		if (temp->next && temp->next->type_token == T_MOT)
			temp->next->type_token = T_FD_IN; // la suite de redir concerne le nom de fichier, donc fd
	}
	else if (temp->type_token == T_RD_OUT)
	{
		if (temp->next && temp->next->type_token == T_MOT)
			temp->next->type_token = T_FD_OUT; // la suite de redir concerne le nom de fichier, donc fd
	}
	else if (temp->type_token == T_RD_APPEND)
	{
		if (temp->next && temp->next->type_token == T_MOT)
			temp->next->type_token = T_FD_OUT_APPEND;
	}
	else if (temp->type_token == T_RD_HEREDOC)
	{
		if (temp->next && temp->next->type_token == T_MOT)
			temp->next->type_token = T_FD_HEREDOC;
	}
}

// pour la condition de token Mot (pour gerer le cas de fd)
void parse_fd_tokens(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		appliquer_token_fd(temp);
		temp = temp->next;
	}
}

// pour tester (enum -> string)	
const char	*get_token_type_str(t_type_token type)
{
	if (type == T_MOT)
		return ("T_MOT");
	else if (type == T_PIPE)
		return ("T_PIPE");
	else if (type == T_RD_IN)
		return ("T_RD_IN");
	else if (type == T_RD_OUT)
		return ("T_RD_OUT");
	else if (type == T_RD_APPEND)
		return ("T_RD_APPEND");
	else if (type == T_RD_HEREDOC)
		return ("T_RD_HEREDOC");
	else if (type == T_FD_IN)
		return ("T_FD_IN"); 
	else if (type == T_FD_OUT)
		return ("T_FD_OUT");
	else if (type == T_FD_HEREDOC)
		return ("T_FD_HEREDOC");
	else if (type == T_FD_OUT_APPEND)
		return ("T_FD_OUT_APPEND");
	return ("inconnu");
}

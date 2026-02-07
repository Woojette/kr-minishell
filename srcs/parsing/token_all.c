#include "minishell.h"

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
	new_node->str = ft_strndup(line, len);
	if (!new_node->str)
	{
		free(new_node);
		return (-1);
	}
	new_node->type_token = type_token;
	new_node->type_quote = GENERAL;
	new_node->type_bi = 0; // par defaut
	new_node->next = NULL;
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

// On parse tout pour trouver les operations ou les builtins
// chaque noeud serait d'abord divise que par soit mot, soit redir, soit pipe  (cf. t_type token)
int parse_input(char *line, t_token **token, t_mini *mini) 
{
	int						len;
	t_type_token	fd_type;

	if (!line || !token || !mini) // proteger au cas ou les pointeurs sont NULL
		return (-1);
	len = 0;
	fd_type = (t_type_token) - 1;
	while (*line)
	{
		if ((*line) == ' ' || (*line) == '\t') // passer l'espace tout au debut
		{
			while ((*line) == ' ' || (*line) == '\t')
				line++;
		}
		else if (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2)) // redirection 
		{
			if (!ft_strncmp(line, ">>", 2))
			{
				if (add_token(line, T_RD_APPEND, 2, token) < 0) // ajouter le token de redirection >> dans la liste chainee
					return (free_tokens(token), -1);
				// on ajoute dans la liste chainee : >>, type : T_RD_APPEND;
				fd_type = T_FD_OUT_APPEND;
			}
			else if (!ft_strncmp(line, "<<", 2))
			{
				if (add_token(line, T_RD_HEREDOC, 2, token) < 0) // ajouter le token de redirection << dans la liste chainee
					return (free_tokens(token), -1);
				fd_type = T_FD_HEREDOC;
			}
			line += 2;
		}
		else if (!ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1))
		{
			if (!ft_strncmp(line, ">", 1))
			{
				if (add_token(line, T_RD_OUT, 1, token) < 0) // ajouter le token de redirection > dans la liste chainee
					return (free_tokens(token), -1);
					// on ajoute dans la liste chainee : >, type : T_RD_OUT;
				fd_type = T_FD_OUT;
			}
			else if (!ft_strncmp(line, "<", 1))
			{
				if (add_token(line, T_RD_IN, 1, token) < 0) // ajouter le token de redirection < dans la liste chainee
					return (free_tokens(token), -1);
				fd_type = T_FD_IN;
			}
			line += 1;
		}
		else if (!ft_strncmp(line, "|", 1)) // pipe  (noeud '|'  /  type : T_PIPE)
		{
			if (fd_type != (t_type_token) - 1)
				return (free_tokens(token), write(2, "syntax error near unexpected token `|'\n", 40), -2);
			if (add_token(line, T_PIPE, 1, token) < 0)
				return (free_tokens(token), -1);
			line += 1;
		}
		else
		{
			len = len_mot_total(line);
			if (len <= 0)
				return (free_tokens(token), -1);
			if (fd_type != (t_type_token) - 1)
			{
				if (add_token(line, fd_type, len, token) < 0)
					return (free_tokens(token), -1);
				fd_type = (t_type_token) - 1;
			}
			else
			{
				if (add_token(line, T_MOT, len, token) < 0)
					return (free_tokens(token), -1);
			}
			line += len;
		}
	}
	if (fd_type != (t_type_token) - 1)
		return (free_tokens(token), write(2, "syntax error near unexpected token `newline'\n", 45), -2);
	if (appliquer_dollar_sur_liste_token(token, mini) == -1) // appliquer dollar en respectant a quotes
		return (free_tokens(token), -1);
	if (appliquer_quote_sur_liste_token(token) == -1) // apres l'expansion de dollar, on supprime quote
		return (free_tokens(token), -1);
	return (0);
}

// pour la condition de token Mot (pour gerer le cas de fd)
void parse_fd_tokens(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
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

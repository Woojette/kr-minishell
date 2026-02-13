#include "minishell.h"

void	passer_espace(char **line) // pour changer la position du pointeur, on utiliser le double pointeur
{
	while ((**line) == ' ' || (**line) == '\t')
		(*line)++;
	return ;
}

int	appliquer_redir_2_len(char **line, t_token **token, t_type_token *fd_type)
{
	if (!ft_strncmp((*line), ">>", 2))
	{
		if (add_token((*line), T_RD_APPEND, 2, token) < 0) // ajouter le token de redirection >> dans la liste chainee
			return (free_tokens(token), -1);
		// on ajoute dans la liste chainee : >>, type : T_RD_APPEND;
		(*fd_type) = T_FD_OUT_APPEND;
		return ((*line) += 2, 1);
	}
	else if (!ft_strncmp((*line), "<<", 2))
	{
		if (add_token((*line), T_RD_HEREDOC, 2, token) < 0) // ajouter le token de redirection << dans la liste chainee
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
		if (add_token((*line), T_RD_OUT, 1, token) < 0) // ajouter le token de redirection > dans la liste chainee
			return (free_tokens(token), -1);
			// on ajoute dans la liste chainee : >, type : T_RD_OUT;
		(*fd_type) = T_FD_OUT;
		return ((*line) += 1, 1);
	}
	else if (!ft_strncmp((*line), "<", 1))
	{
		if (add_token((*line), T_RD_IN, 1, token) < 0) // ajouter le token de redirection < dans la liste chainee
			return (free_tokens(token), -1);
		(*fd_type) = T_FD_IN;
		return ((*line) += 1, 1);
	}
	return (0);
}

int	appliquer_redir_token(char **line, t_token **token, t_type_token *fd_type)
{
	if (!ft_strncmp((*line), ">>", 2) || !ft_strncmp((*line), "<<", 2)) // redirection 
		return (appliquer_redir_2_len(line, token, fd_type));
	else if (!ft_strncmp((*line), ">", 1) || !ft_strncmp((*line), "<", 1))
		return (appliquer_redir_1_len(line, token, fd_type));
	return (0);
}

int	appliquer_pipe_token(char **line, t_token **token, t_type_token *fd_type)
{
	if ((*fd_type) != (t_type_token) - 1)
		return (free_tokens(token), write(2, "syntax error near unexpected token `|'\n", 40), -2);
	if (add_token((*line), T_PIPE, 1, token) < 0)
		return (free_tokens(token), -1);
	(*line) += 1;
	return (0);
}

// int	appliquer_redir_pipe(char **line, t_token **token, t_type_token *fd_type)
// {
// 	int	resultat;

// 		if (!ft_strncmp((*line), ">>", 2) || !ft_strncmp((*line), "<<", 2)
// 			|| !ft_strncmp((*line), ">", 1) || !ft_strncmp((*line), "<", 1))
// 		{
// 			resultat = appliquer_redir_token(line, token, fd_type);
// 			if (resultat == -1)
// 				return (-1);
// 			else if (resultat == 1)
// 				continue ;
// 		}
// 		else if (!ft_strncmp((*line), "|", 1)) // pipe  (noeud '|'  /  type : T_PIPE)
// 		{
// 			resultat = appliquer_pipe_token(line, token, fd_type);
// 			if (resultat < 0)
// 				return (resultat);
// 			else
// 				continue ;
// 		}
// }

int	appliquer_mot_token(char **line, t_token **token, t_type_token *fd_type, int *len)
{
	(*len) = len_mot_total((*line));
	if ((*len) <= 0)
		return (free_tokens(token), -1);
	if ((*fd_type) != (t_type_token) - 1)
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
	if (fd_type != (t_type_token) - 1)
		return (free_tokens(token), write(2, "syntax error near unexpected token `newline'\n", 45), -2);
	if (appliquer_dollar_sur_liste_token(token, mini) == -1) // appliquer dollar en respectant a quotes
		return (free_tokens(token), -1);
	if (appliquer_quote_sur_liste_token(token) == -1) // apres l'expansion de dollar, on supprime quote
		return (free_tokens(token), -1);
	return (0);
}

// On parse tout pour trouver les operations ou les builtins
// chaque noeud serait d'abord divise que par soit mot, soit redir, soit pipe  (cf. t_type token)
// int parse_input(char *line, t_token **token, t_mini *mini) 
// {
// 	int						len;
// 	int						resultat;
// 	t_type_token	fd_type;

// 	if (!line || !token || !mini) // proteger au cas ou les pointeurs sont NULL
// 		return (-1);
// 	len = 0;
// 	fd_type = (t_type_token) - 1;
// 	while (*line)
// 	{
// 		if ((*line) == ' ' || (*line) == '\t') // passer l'espace tout au debut
// 			passer_espace(&line);
// 		else if (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2)
// 			|| !ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1))
// 		{
// 			resultat = appliquer_redir_token(&line, token, &fd_type);
// 			if (resultat == -1)
// 				return (-1);
// 			else if (resultat == 1)
// 				continue ;
// 		}
// 		else if (!ft_strncmp(line, "|", 1)) // pipe  (noeud '|'  /  type : T_PIPE)
// 		{
// 			resultat = appliquer_pipe_token(&line, token, &fd_type);
// 			if (resultat < 0)
// 				return (resultat);
// 			else
// 				continue ;
// 		}
// 		else
// 		{
// 			if (appliquer_mot_token(&line, token, &fd_type, &len) < 0)
// 				return (-1);
// 		}
// 	}
// 	resultat = appliquer_token_final(token, fd_type, mini);
// 	return (resultat);
// }


int	est_espace(char c)
{
	return (c == ' ' || c == '\t');
}

int	est_redirection(char *line)
{
	return (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2)
		|| !ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1));
}

int	est_pipe(char *line)
{
	return (!ft_strncmp(line, "|", 1));
}

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

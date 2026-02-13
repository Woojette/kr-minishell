#include "minishell.h"

// appliquer le remplacement de $ pour chaque token de type T_MOT
// ex) $USER -> username
int	appliquer_dollar_sur_liste_token(t_token **token, t_mini *mini)
{
	char		*new_str; // le nouveau str apres remplacement de $
	t_token	*temp;

	if (!token || !(*token) || !mini)
		return (-1);
	temp = *token;
	while (temp) // parcourir toute la liste chainee token
	{
		if (temp->type_token == T_MOT || temp->type_token == T_FD_IN
			|| temp->type_token == T_FD_OUT || temp->type_token == T_FD_OUT_APPEND)	// si le type de token est T_MOT et redir -> on applique le remplacement de $ (heredoc sera gere ailleurs)
		{
			if (!temp->str) // si str est NULL, on retourne -1 (erreur)
				return (-1);
			new_str = remplacer_dollar(temp->str, mini); // remplacer $ par la valeur de la variable d'env
			if (!new_str)
				return (-1);
			free(temp->str);
			temp->str = new_str; // on met a jour token->str avec le nouveau str
		}
		temp = temp->next; // passer au noeud suivant
	}
	return (0);
}

// enlever des quotes pour chaque token de type T_MOT et fd redir
int	appliquer_quote_sur_liste_token(t_token **token)
{
	char		*new_str; // le nouveau str apres enlever les quotes
	t_token	*temp;

	if (!token || !(*token)) // si token n'existe pas
		return (-1);
	temp = *token; // initialiser temp avec le debut de la liste chainee token
	while (temp) // parcourir toute la liste chainee token
	{
		if (temp->str && (temp->type_token == T_MOT || temp->type_token == T_FD_IN
			|| temp->type_token == T_FD_OUT || temp->type_token == T_FD_OUT_APPEND
			|| temp->type_token == T_FD_HEREDOC))	// on enleve les quotes si le type de token est T_MOT ou redir
		{
			new_str = enlever_quote_dans_token(temp->str); // enlever les quotes dans token->str
			if (!new_str)
				return (-1);
			free(temp->str);
			temp->str = new_str; // on met a jour token->str avec le nouveau str
		}
		temp = temp->next; // passer au noeud suivant
	}
	return (0);
}

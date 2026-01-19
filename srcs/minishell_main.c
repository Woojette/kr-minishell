#include "minishell.h"

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

// On ajoute le token dans la structure t_token;
void add_token(char *line, t_type_token type_token, int len, t_token **token)
{
    t_token *new_node;
    t_token *tmp;

    new_node = malloc(sizeof(t_token));
    if (!new_node)
        return ;

    new_node->str = ft_strndup(line, len);    
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
}

// liste free
void	free_tokens(t_token **token)
{
	t_token	*tmp;

	while (*token)
	{
		tmp = *token;
		*token = (*token)->next;
		free(tmp->str);
		free(tmp);
	}
}

// // On parse pour les mots pour trouver les builtins, on ajoute dans la structure si on en trouve
// void parse_builtin(char *line)
// {
// 	if (!ft_strncmp(line, "echo", 4))
// 		add_token(line, T_ECHO, 4);
// 	else if (!ft_strncmp(line, "cd", 2))
// 		add_token(line, T_CD, 2);
// 	else if (!ft_strncmp(line, "pwd", 3))
// 		add_token(line, T_PWD, 3);
// 	else if (!ft_strncmp(line, "export", 6))
// 		add_token(line, T_EXPORT, 6);
// 	else if (!ft_strncmp(line, "unset", 5))
// 		add_token(line, T_UNSET, 5);
// 	else if (!ft_strncmp(line, "env", 3))
// 		add_token(line, T_ENV, 3);
// 	else if (!ft_strncmp(line, "exit", 4))
// 		add_token(line, T_EXIT, 4);
// }

// compter le nombre de caracteres s'il y a pas de 2 quotes qui fonctionnent
int	len_mot_sans_quote(char *line)
{
	int	i;

	i = 0;
	while ((*line) && (*line) != ' ' && (*line) != '\t'
		&& (*line) != '>' && (*line) != '<' && (*line) != '|'
		&& (*line) != '"' && (*line) != '\'')
	{
		line++;
		i++;
	}
	return (i);
}

// Verifie les quotes, dès qu’on croise une quote, on cherche sa paire
// si on arrive a la fin avant = unclose quote
// a verifier avant de commencer le parsing
int	check_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	if (!line[i])
		return (0);
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'') // si on croise une quote, on entre dans la boucle pour trouver une autre
		{
			quote = line[i]; // sauvegrader la premiere quote
			i++; // puis, commencer a chercher la deuxieme
			while (line[i] && line[i] != quote) // ce qui n'est pas une quote pareille que la premiere, on passe
				i++;
			if (!line[i]) // on est a la fin sans trouver la quote fermante -> 0
				return (0);
			else
				return (1);
		}
		i++;
	}
	return (1); // s'il y a 2 quotes bien fermees -> 1
}


// verifier s'il y a 2 quotes pareils dans la chaine de caracteres
// on recupere le type du premier quote 
// soit return (1); ex) " ", ' ',  " ' "  , soit return (0); ex)  ', ", "', '", "'', '"", "'''', '''"
// int	check_quote_debut_ok(char *line) 
// {
	// char	debut_quote; // on recupere le type du premier quote ( " ou ' )
	// int		nbr_dquote;
	// int		nbr_squote;
	// int		i; // l'index pour parcourir la chaine de caracteres
// 
	// nbr_dquote = 0;
	// nbr_squote = 0;
	// if (line[0] == '"') // si la chaine de caractere commence par double quote
	// {
		// debut_quote = 'd';
		// nbr_dquote++;
	// }
	// else if (line[0] == '\'') // si la chaine de caractere commence par single quote
	// {
		// debut_quote = 's';
		// nbr_squote++;
	// }
	// i = 1;
	// while (line[i])
	// {
		// if (debut_quote == 'd' && line[i] == '"') // verifier le cas " "
			// nbr_dquote++;
		// else if (debut_quote == 's' && line[i] == '\'') // verifier le cas ' '
			// nbr_squote++;
		// if ((nbr_dquote == 2) || (nbr_squote == 2))
			// break ;
		// i++;
	// }
	// if ((nbr_dquote == 2) || (nbr_squote == 2)) // si " " ou ' ' -> return (1)
		// return (1);
	// return (0);
// }

// verifier s'il y a 2 quotes pareils dans la chaine de caracteres
// on recupere le type du premier quote 
// soit return (1); ex) " ", ' ',  " ' "  , soit return (0); ex)  ', ", "', '", "'', '"", "'''', '''"
int	check_quote_debut_ok(char *line) 
{
	char	debut_quote; // on recupere le type du premier quote ( " ou ' )
	// int		nbr_dquote;
	// int		nbr_squote;
	int		compter_quote; // compter le nombre de quotes (qui est bien 2)
	int		i; // l'index pour parcourir la chaine de caracteres

	// nbr_dquote = 0;
	// nbr_squote = 0;
	if (!line || (line[0] != '"' && line[0] != '\''))
		return (0);
	debut_quote = line[0];
	if (line[0] == '"' || line[0] == '\'')
		compter_quote = 1;
	else
		return (0);
	i = 1;
	while (line[i])
	{
		if (line[i] == debut_quote)
		{
			compter_quote++;
			break ;
		}
		i++;
	}
	if (compter_quote == 2) // si " " ou ' ' -> return (1)
		return (1);
	return (0);
}

// // verifier s'il y a 2 quotes pareils dans la chaine de caracteres
// // on recupere le type du premier quote 
// // soit return (1); ex) " ", ' ',  " ' "  , soit return (0); ex)  ', ", "', '", "'', '"", "'''', '''"
// int	check_quote_debut_ok(char *line) 
// {
//     char debut_quote;
//     int i, quote_count;

//     if (!line || (line[0] != '"' && line[0] != '\''))
//         return 0; // 따옴표로 시작하지 않으면 OK

//     debut_quote = line[0];
//     quote_count = 1;
//     i = 1;
//     while (line[i]) {
//         if (line[i] == debut_quote)
//             quote_count++;
//         if (quote_count == 2)
//             return 1; // 정상적으로 닫힘
//         i++;
//     }
//     // 여기까지 오면 닫는 따옴표가 없음
//     return 0;
// }

// fonction qui verifie l'espace (ou '\0', reir, pipe) apres la 2e quote
// au cas ou le premier caractere commence par une quote
int check_2_quotes_debut_puis_fin(char *line)
{
	int		i; // l'index pour parcourir la chaine line
	char	quote; // pour sauvegarder la premiere quote (qui est le premier caractere de la chaine)

	quote = line[0];
	i = 1;
	if (check_quote_debut_ok(line) == 1) // si on a 2 quotes pareilles  ex) "...", '...'
	{
		while (line[i])
		{
			if (line[i] == quote) // quand on trouve la 2e quote, on arrete
				break ;
			i++;
		}
		i++; // pour verifier le caractere apres la 2e quote
		if (line[i] == ' ' || line[i] == '\0'
			|| line[i] == '>' || line[i] == '<' || line[i] == '|') // si on a l'espace (ou '\0', redir, pipe) apres 2e quote -> 1
			return (1);     // ex) echo "hihi"  coucou
	}
	return (0); // quotes puis caractere -> 0  ex) echo "hihi"coucou
}

// compter le nombre de caracteres entre 2 quotes, y compris les 2 quotes  ex) "...", '...' 
int	len_mot_2_quotes_entier(char *line)
{
	char	debut_quote; // recuperer la quote de premier caractere de la chaine
	int		i;
	int		len;

	debut_quote = caractere_quote_debut(line);
	i = index_quote_debut(line, debut_quote) + 1;
	len = 1;
	while (line[i])
	{
		if (line[i] == debut_quote)
		{
			len++;
			break ;
		}
		len++;
		i++;
	}
	return (len);
}

// verifier s'il y a 2 quotes identiques dans la chaine de caracteres.
// (pas le premier caractere, au milieu ou a la fin)
// ex) echo you"p"i, echo you"pi"
int	check_quote_milieu_ok(char *line)
{
	char	debut_quote; // recuperer la premiere quote dans la chaine de caracteres
	int		compte_debut_quote; // compter le nombre de debut_quote
	int		i; // l'index pour parcourir la chaine

	debut_quote = 0;
	compte_debut_quote = 0;
	i = 0;
	while (line[i]) // pour recuperer la premiere quote
	{
		if (line[i] == '"' || line[i] == '\'') 
		{
			debut_quote = line[i]; // sauvegarder la premiere quote
			compte_debut_quote++; // compter le nombre de la premiere quote (d'abord 1)
			break;
		}
		i++;
	}
	i = 0; // reinitialiser l'index
	while (line[i]) // verifier si on a 2 quotes identiques (qu'on a eu au debut de la chaine)
	{
		if (line[i] == debut_quote) // si on trouve la meme quote (du debut) +1
			compte_debut_quote++;
		if (compte_debut_quote == 2) // ex) echo you"pi"ii, echo you"pi"
			return (1);
		i++;
	}
	return (0); // ex) echo you"pi''
}

// verifier s'il y a un espace (redir, pipe) avant la premiere quote
int check_avant_quote_espace(char *line)
{
    int i;
    int pos_quote;

    if (!line || !*line)
        return 0;
    // cherecher la premiere quote
    i = 0;
    while (line[i] && line[i] != '"' && line[i] != '\'')
        i++;
    if (!line[i])
        return (0);
    pos_quote = i;  // l'index de la premiere quote
    if (pos_quote == 0) // s'il y a pas de quote dans la chaine, pas besoin de chercher
        return (1);
    i = pos_quote - 1; // avant la quote
    while (i >= 0) // on cherche un espace (redir, pipe) jusqu'a tout au debut de la chaine
    {
        if (line[i] == ' ' || line[i] == '>' || line[i] == '<' || line[i] == '|')
            return 1;
        i--;
    }
    return (0);
}

// recuprer le caractere de la premiere quote
char	caractere_quote_debut(char *line)
{
	int		i;
	char	debut_quote;

	i = 0;
	debut_quote = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			debut_quote = line[i];
			break ;
		}
		i++;
	}
	return (debut_quote);
}

// recuperer l'index de la premiere quote 
int	index_quote_debut(char *line, char c)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == c)
			break ;
		i++;
	}
	return (i);
}

// recuperer l'index de la deuxieme quote 
int	index_quote_fin(char *line, char c)
{
	int	i;
	int	debut_quote;

	debut_quote = index_quote_debut(line, c);
	i = debut_quote + 1;
	while (line[i])
	{
		if (line[i] == c)
			break ;
		i++;
	}
	return (i);
}

// fonction qui verifie l'espace (ou '\0', redir, pipe) apres la 2e quote
// au cas ou le premier caractere ne commence pas par une quote
int check_2_quotes_milieu_puis_fin(char *line)
{
	int		i;
	int		compter_quote;
	char	debut_quote; // pour sauvegarder la premiere quote

	compter_quote = 0;
	debut_quote = caractere_quote_debut(line);
	i = index_quote_debut(line, debut_quote);
	// si on a 2 quotes pareilles au milieu ou a la fin  
	// ex) echo hi"hi", echo hi'hi' | cat -e
	if (check_quote_milieu_ok(line) == 1)
	{
		compter_quote++;
		i++;
		while (line[i])
		{
			if (line[i] == debut_quote)
				compter_quote++;
			if (compter_quote == 2)
				break ;
			i++;
		}
		if (compter_quote != 2) // pour proteger au cas ou il y a qu'une seule quote dans la chaine
			return (0);
		i++; // pour verifier le caractere apres la 2e quote
		if (line[i] == ' ' || line[i] == '\0'
			|| line[i] == '>' || line[i] == '<' || line[i] == '|') // si on a l'espace (ou '\0', redir, pipe) apres 2e quote -> 1
			return (1);   // ex) echo you"pi", echo you'pi' | cat -e
	}
	return (0); // quotes puis caractere -> 0  ex) echo you"p"i
}

// recuperer len avant la quote qui est au milieu de la chaine
int	len_mot_avant_quote(char *line)
{
	int		i;
	int		quote;
	char	debut_quote;

	i = 0;
	debut_quote = caractere_quote_debut(line);
	quote = index_quote_debut(line, debut_quote);
	while (i < quote)
		i++;
	return (i); // ex) you"pi" (-> 3)
}

// recuperer len apres la 2e quote
int	len_mot_apres_quote(char *line)
{
	int		i;
	int		len_apres_quote;
	char	quote;

	quote = caractere_quote_debut(line);
	i = index_quote_fin(line, quote) + 1;
	len_apres_quote = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\0'
			|| line[i] == '>' || line[i] == '<' || line[i] == '|')
			break;
		len_apres_quote++;
		i++;
	}
	return (len_apres_quote);
}

// // compter len du type mot (avec quote + sans quote)
// int	len_mot_total(char *line)
// {
	// int	len;
// 
	// len = 0;
	// 1. le cas ou le premier caractere est une quote  ex) "hihi", "hihi, 'hihi', 'hihi, 'hi"hi, etc
	// if (line[0] == '"' || line[0] == '\'')
	// {
		// 1-1.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) fin (' ' ou '\\0' ou redir ou pipe) apres la 2e quote
		// if (check_quote_debut_ok(line) == 1 && check_2_quotes_debut_puis_fin(line) == 1) // ex) echo "hihi" coucou,  echo "hihi", echo "hihi"| ~~~
			// len = len_mot_2_quotes_entier(line); // calcule a partir de la 1e quote a la 2e quote  ex) "..."
		// 1-2.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) un caractere apres la 2e quote
		// else if (check_quote_debut_ok(line) == 1 && check_2_quotes_debut_puis_fin(line) == 0)
			// len = len_mot_2_quotes_entier(line) + len_mot_apres_quote(line); // ex) 'you'pi ->  strlen("'you'") + strlen("pi")
		// 1-3.  1) le premier caractere = quote   2) 2 quotes pas fermees
		// else if (check_quote_debut_ok(line) == 0) // ex) ', ", "', '", "'', '"", "'''', '''", echo "hi, echo 'hi |
			// len = len_mot_sans_quote(line);
	// }
	// 2. le cas ou le premier caractere ne commence pas par une quote (mais pas redir, ni pipe non plus)
	// else if (line[0] != '"' || line[0] != '\'')
	// {
		// 2-1.  1) quote au milieu   2) 2 quotes bien fermees   3) apres quote ' ' ou redir ou pipe
		// if (check_quote_milieu_ok(line) == 1 && check_avant_quote_espace(line) == 0 && check_2_quotes_milieu_puis_fin(line) == 1)
		// {
			// // printf("test  2-1a\\n");
			// len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line);
		// }
		// 2-2.  1) quote au milieu   2) 2 quotes bien fermees   3) caractere apres la 2e quote
		// else if (check_quote_milieu_ok(line) == 1 && check_avant_quote_espace(line) == 0 &&  check_2_quotes_milieu_puis_fin(line) == 0)
		// {
			// // printf("test  2-2\\n");
			// len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line) + len_mot_apres_quote(line);
		// }
		// 2-3.  1) pas de quote
		// else if (check_quote_milieu_ok(line) == 0 && check_avant_quote_espace(line) == 0)
		// {
			// // printf("test 2-3\\n");
			// len = len_mot_sans_quote(line);
		// }
		// 2-4. proteger au cas ou on sait jamais
		// else
		// {
			// // printf("test 2-4\\n");
			// len = len_mot_sans_quote(line);
		// }
	// }
	// return (len);
// }

// compter len du type mot (avec quote + sans quote)
int	len_mot_total(char *line)
{
	int	len;

	len = 0;
	// 1. le cas ou le premier caractere est une quote  ex) "hihi", "hihi, 'hihi', 'hihi, 'hi"hi, etc
	if (line[0] == '"' || line[0] == '\'')
	{
		// 1-1.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) fin (' ' ou '\\0' ou redir ou pipe) apres la 2e quote
		if (check_quotes(line) == 1 && check_2_quotes_debut_puis_fin(line) == 1) // ex) echo "hihi" coucou,  echo "hihi", echo "hihi"| ~~~
			len = len_mot_2_quotes_entier(line); // calcule a partir de la 1e quote a la 2e quote  ex) "..."
		// 1-2.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) un caractere apres la 2e quote
		else if (check_quotes(line) == 1 && check_2_quotes_debut_puis_fin(line) == 0)
			len = len_mot_2_quotes_entier(line) + len_mot_apres_quote(line); // ex) 'you'pi ->  strlen("'you'") + strlen("pi")
		// 1-3.  1) le premier caractere = quote   2) 2 quotes pas fermees
		else if (check_quotes(line) == 0) // ex) ', ", "', '", "'', '"", "'''', '''", echo "hi, echo 'hi |
			len = len_mot_sans_quote(line);
	}
	// 2. le cas ou le premier caractere ne commence pas par une quote (mais pas redir, ni pipe non plus)
	else if (line[0] != '"' || line[0] != '\'')
	{
		// 2-1.  1) quote au milieu   2) 2 quotes bien fermees   3) apres quote ' ' ou redir ou pipe
		if (check_quotes(line) == 1 && check_avant_quote_espace(line) == 0 && check_2_quotes_milieu_puis_fin(line) == 1)
		{
			// printf("test  2-1a\\n");
			len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line);
		}
		// 2-2.  1) quote au milieu   2) 2 quotes bien fermees   3) caractere apres la 2e quote
		else if (check_quotes(line) == 1 && check_avant_quote_espace(line) == 0 &&  check_2_quotes_milieu_puis_fin(line) == 0)
		{
			// printf("test  2-2\\n");
			len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line) + len_mot_apres_quote(line);
		}
		// 2-3.  1) pas de quote
		else if (check_quotes(line) == 0 && check_avant_quote_espace(line) == 0)
		{
			// printf("test 2-3\\n");
			len = len_mot_sans_quote(line);
		}
		// 2-4. proteger au cas ou on sait jamais
		else
		{
			// printf("test 2-4\\n");
			len = len_mot_sans_quote(line);
		}
	}
	return (len);
}


// On parse tout pour trouver les operations ou les builtins
// chaque noeud serait d'abord divise que par soit mot, soit redir, soit pipe  (cf. t_type token)
void parse_input(char *line, t_token **token) 
{
	int	len;

	len = 0;
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
				add_token(line, T_RD_APPEND, 2, token); // on ajoute dans la liste chainee : >>, type : T_RD_APPEND;
			else if (!ft_strncmp(line, "<<", 2))
				add_token(line, T_RD_HEREDOC, 2, token);
			line += 2;
		}
		else if (!ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1))
		{
			if (!ft_strncmp(line, ">", 1))
				add_token(line, T_RD_OUT, 1, token);
			else if (!ft_strncmp(line, "<", 1))
				add_token(line, T_RD_IN, 1, token);
			line += 1;
		}
		else if (!ft_strncmp(line, "|", 1)) // pipe  (noeud '|'  /  type : T_PIPE)
		{
			add_token(line, T_PIPE, 1, token);
			line += 1;
		}
		else
		{
			len = len_mot_total(line);
			add_token(line, T_MOT, len, token);
			line += len;
		}
	}
}

// pour la condition de token
void parse_fd_tokens(t_token **token)
{
	t_token	*temp;

	temp = *token;
	while (temp)
	{
		if (temp->type_token == T_RD_IN)
		{
			if (temp->next && temp->next->type_token == T_MOT)
				temp->next->type_token = T_FD_IN;
		}
		else if (temp->type_token == T_RD_OUT)
		{
			if (temp->next && temp->next->type_token == T_MOT)
				temp->next->type_token = T_FD_OUT;
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

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	char	*line;
	t_token	*parsing;
	t_token	*temp;
	int		i;
	// t_minis	*mini;
	// int		j;

	// mini = malloc(sizeof(mini));
	// if (!mini)
	// 	return (0);
	i = 0;
	while (1)
	{
		line = readline("coucou$ ");
		if (!line)
			break ;
		parsing = NULL;
		i = 0;
		parse_input(line, &parsing);
		temp = parsing;
		while (temp)
		{
			printf("noeud %d '%s' | type %s\n", i, temp->str, get_token_type_str(temp->type_token));
			i++;
			temp = temp->next;
		}
		free_tokens(&parsing);
		free(line);
	}
	return (0);
}

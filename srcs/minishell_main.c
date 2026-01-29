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

// pour tester type quote
char	*get_token_type_state(t_state state)
{
	if (state == GENERAL)
		return ("GENERAL");
	else if (state == DQUOTES)
		return ("DQUOTES");
	else if (state == SQUOTES)
		return ("SQUOTES");
	return ("inconnu");
}

// On ajoute le token dans la structure t_token;
void add_token(char *line, t_type_token type_token, int len, t_token **token)
{
    t_token *new_node;
    t_token *tmp;
		// char		temp_quote;

		// temp_quote = 0;
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
	if (!line[i]) // si la chaine est vide (line[0] = '\0')
		return (0);
	while (line[i])
	{
		// a partir de la, on cherche les quotes
		if (line[i] == '"' || line[i] == '\'') // si on croise une quote, on entre dans la boucle pour trouver une autre
		{
			quote = line[i]; // sauvegarder la premiere quote
			i++; // puis, commencer a chercher la deuxieme
			while (line[i] && line[i] != quote) // ce qui n'est pas une quote pareille que la premiere, on passe
				i++;
			// on sort de la boucle soit quand on trouve la quote pareille (bien fermee), soit a la fin de la chaine
			if (!line[i]) // on est a la fin de la chaine ('\0'), sans trouver la quote fermante -> 0
				return (0);
		}
		i++; 
		// soit on a trouve la quote fermante, soit on n'a pas encore trouve de quote, 
		// on continue a parcourir le caractere suivant de la chaine
		// ca permet de verifier toutes les quotes dans la chaine (ex. echo "hihi"coucou'you'pi)
	}
	return (1); // si les quotes dans la chaine sont appariees et fermees -> 1
}

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

// fonction qui verifie l'espace (ou '\0', redir, pipe) apres la 2e quote
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

// compter len du type mot (avec quote + sans quote)
int	len_mot_total(char *line)
{
	int	len;

	len = 0;
	// 1. le cas ou le premier caractere est une quote  ex) "hihi", "hihi, 'hihi', 'hihi, 'hi"hi, etc
	if (line[0] == '"' || line[0] == '\'')
	{
		// 1-1.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) fin (' ' ou '\\0' ou redir ou pipe) apres la 2e quote
		if (check_quote_debut_ok(line) == 1 && check_2_quotes_debut_puis_fin(line) == 1) // ex) echo "hihi" coucou,  echo "hihi", echo "hihi"| ~~~
			len = len_mot_2_quotes_entier(line); // calcule a partir de la 1e quote a la 2e quote  ex) "..."
		// 1-2.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) un caractere apres la 2e quote
		else if (check_quote_debut_ok(line) == 1 && check_2_quotes_debut_puis_fin(line) == 0)
			len = len_mot_2_quotes_entier(line) + len_mot_apres_quote(line); // ex) 'you'pi ->  strlen("'you'") + strlen("pi")
		// 1-3.  1) le premier caractere = quote   2) 2 quotes pas fermees
		else if (check_quote_debut_ok(line) == 0) // ex) ', ", "', '", "'', '"", "'''', '''", echo "hi, echo 'hi |
			len = len_mot_sans_quote(line);
	}
	// 2. le cas ou le premier caractere ne commence pas par une quote (mais pas redir, ni pipe non plus)
	else if (line[0] != '"' || line[0] != '\'')
	{
		// 2-1.  1) quote au milieu   2) 2 quotes bien fermees   3) apres quote ' ' ou redir ou pipe
		if (check_quote_milieu_ok(line) == 1 && check_avant_quote_espace(line) == 0 && check_2_quotes_milieu_puis_fin(line) == 1)
		{
			// printf("test  2-1a\\n");
			len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line);
		}
		// 2-2.  1) quote au milieu   2) 2 quotes bien fermees   3) caractere apres la 2e quote
		else if (check_quote_milieu_ok(line) == 1 && check_avant_quote_espace(line) == 0 &&  check_2_quotes_milieu_puis_fin(line) == 0)
		{
			// printf("test  2-2\\n");
			len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line) + len_mot_apres_quote(line);
		}
		// 2-3.  1) pas de quote
		else if (check_quote_milieu_ok(line) == 0 && check_avant_quote_espace(line) == 0)
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

// verifier s'il y a un pipe a la fin de la chaine ou l'espace seulement apres le dernier pipe
int	check_pipe_fin(char *line)
{
	int	i;

	i = 0;
	while (line[i]) // une boucle pour arriver a la fin de la chaine
		i++;
	i--; // quand on sort la boucle, c'est '\0'. donc on avance une fois
	while (i >= 0 && line[i] == ' ') // avancer s'il y a l'espace a la fin
		i--;
	if (i >= 0 && line[i] == '|') // verifier s'il y a un pipe apres l'espace
		return (1);
	return (0);
}

// compter le nombre de pipes dans la liste chainee
int count_pipe(t_token *token)
{
	int			count;
	t_token	*temp; // temporaire pour parcourir liste chainee token

	count = 0;
	temp = token;
	while (temp)
	{
		if (temp->type_token == T_PIPE)
			count++;
		temp = temp->next;
	}
	return (count);
}

// allouer le tableau de structures cmd (divise par pipe)
// on va mettre chaque commande divisee par pipe dans chaque cmd[i].cmd
t_cmd	*malloc_cmd(t_token *token)
{
	int		compter_pipe; // le nombre de pipes dans la ligne de commande
	int		nbr_cmd; // le nombre de commandes a allouer le memoire
	int		j; // index pour parcourir le tableau de structures cmd
	t_cmd	*cmd; // le tableau de structures pour chaque commande divisee par pipe

	j = 0;
	compter_pipe = count_pipe(token); // compter le nombre de pipes
	nbr_cmd = compter_pipe + 1; // le nombre de commandes = le nombre de pipes + 1 
	// ex) echo "hihi" | cat -e | cat -e  <- 2 pipes, donc 3 commandes
	cmd = malloc(sizeof(t_cmd) * nbr_cmd); // allouer un tableau de structures t_cmd (une par commande)
	if (!cmd)
		return (NULL);
	while (j < nbr_cmd) // j est index, donc ca commence par 0
	{
		cmd[j].cmd = NULL; // on initialise tous les pointeurs a NULL (pour proteger)
		j++;
	}
	return (cmd);
}

// fonction pour agrandir un tableau et rajouter une chaine, comme pour le builtin export
// char **tab : double tableau actuel, *str : nouveau tableau a ajouter,
// int  size : taille actuelle de double tableau
char** add_double_tab(char **tab, char *str, int size)
{
    char **new_tab; // nouveau double tableau agrandi
    int j; // index pour parcourir les tableaux

    new_tab = malloc(sizeof(char *) * (size + 2)); // +1 pour le nouveau +1 pour NULL hihi
    if (!new_tab)
        return (NULL);
    j = 0;
    while (j < size)
    {
        new_tab[j] = tab[j]; // copier l'adresse de chaque chaine
				// chaque tab[j] est un pointeur vers une chaine de caracteres
        // cf) char **tab = *tab[] = {"pho", "malatang", NULL}; (tableau de pinteurs vers des chaines)
				//     char tab[j] -> tab[0] = adresse de "pho", tab[1] = adresse de "malatang" (chaine de caracteres)
				j++;
    }
    new_tab[j] = str; // l'adresse de nouvelle chaine ajoutee, on ajoute la nouvelle adresse ici youpiii
    new_tab[j + 1] = NULL; // terminer par NULL
    free(tab); // vu qu'on a bien cree un nouveau tableau agrandi, on libere l'ancien tableau de pointeurs
    return (new_tab);
}

// parcourir les token, et rajouter les token dans les tableaux
// ex) echo hihi | cat -e
// l'objectif, c'est de mettre  tab[0] = {"echo", "hihi", NULL}, tab[1] = {"cat", "-e", NULL}  dans la liste chainee cmd
// ( remplir  cmd[0].cmd = {"echo","hihi",NULL}, cmd[1].cmd = {"cat","-e",NULL} )
int add_cmd(t_token *token, t_cmd *cmd)
{
	int index_cmd; // l'index pour la structure  ex) tab[0] = {"echo", "hihi", NULL}, tab[1] = {"cat", "-e", NULL}
	int i; // l'index pour l'argument de chaque structure  ex) tab[0][0] = "echo", tab[0][1] = "hihi", tab[0][2] = NULL

	index_cmd = 0;
	i = 0;
	while (token) // pendant que le noeud dans la liste chainee existe
	{
		if (token->type_token == T_MOT) // si le type de token est T_MOT
		{
			if (cmd[index_cmd].cmd == NULL) // si le tableau cmd[index_cmd].cmd n'est pas encore alloué (NULL)
			{
				cmd[index_cmd].cmd = malloc(sizeof(char *) * 2); 
				// initialement allouer pour 2 cases (tab[0] = "~~" , tab[1] = NULL)
				if (!cmd[index_cmd].cmd) 
					return (-1);
				cmd[index_cmd].cmd[1] = NULL; // vu que c'est un double tableau (tableau de pointeurs char *, donc argv), on place d'abord le NULL final
			}
			else // s'il y a deja un argument dans le double tableau, on agrandit le tableau (pour ajouter un nouvel arguement)
			{
				cmd[index_cmd].cmd = add_double_tab(cmd[index_cmd].cmd, NULL, i); 
				// ex) tab[0] = {"echo", NULL} -> {"echo", "hihi", NULL} 
				if (!cmd[index_cmd].cmd)
					return (-1);
			}
			cmd[index_cmd].cmd[i] = ft_strdup(token->str); // on ajoute le contenu de token a cet argument de la telle structure 
			i++; // pour passer au prochain argument, incrementer de 1
		}
		if (token->type_token == T_PIPE) // si on arrive a '|'
		{
			if(cmd[index_cmd].cmd == NULL) // proteger au cas ou il y a 2 pipes consecutifs (ex: cmd1 || cmd2)
				return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
			cmd[index_cmd].cmd[i] = NULL; // on met le NULL terminateur pour cloturer argv
			index_cmd++; // on passe a la structure suivante
			i = 0; // pour reverifier des le debut, on reinitialise l'index pour l'argument de cette nouvelle structure
		}
		token = token->next; // on passe au noeud suivant
	}
	if (index_cmd > 0 && cmd[index_cmd].cmd == NULL) // proteger au cas ou il y a un pipe a la fin (ex: cmd1 | )
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	if (cmd[index_cmd].cmd != NULL) // on termine aussi le dernier argv (apres la boucle)
		cmd[index_cmd].cmd[i] = NULL; // on ferme bien la fin 
	return (0); // pour distinguer du cas qui marche bien le cas d'erreur (-1)
}

// tester le contenu de chaque cmd
void test_print_cmds(t_cmd *cmd, int nbr_cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < nbr_cmd)
	{
		printf("command%d:\n", i);
		while (cmd[j].cmd && cmd[j].cmd[i] != NULL)
		{
			printf("arg %d: %s\n", j, cmd[j].cmd[i]);
			i++;
		}
		j++;
		i = 0;
	}
}

// // decouper des commandes par pipe (on les sauvegrade dans le double tableau)
// char	**split_input_par_pipe(char *line)
// {
// 	int		compter_pipe; // compter le nombre de pipe (pour size de double tableau)
// 	int		size_cmd;
// 	int		i;
// 	int		j;
// 	int		start;
// 	char	**cmd; // double tableau decoupe par pipe

// 	compter_pipe = 0;
// 	size_cmd = 0;
// 	i = 0;
// 	j = 0;
// 	start = 0;
// 	cmd = NULL;
// 	if (!line || !line[i])
// 		return (NULL);
// 	while (line[i])
// 	{
// 		if (line[i] == '|')
// 			compter_pipe++;
// 		i++;
// 	}
// 	if (line[i-1] == '|' || check_pipe_fin(line) == 1) // soit le pipe a la fin, soit l'espace seulement apres le dernier pipe
// 		size_cmd = compter_pipe;
// 	else
// 		size_cmd = compter_pipe + 1;
// 	cmd = malloc(sizeof(char *) * (size_cmd + 1));
// 	if (!cmd)
// 		return (NULL);
// 	i = 0;
// 	while (line[i])
// 	{
//     if (line[i] == '|')
//     {
//         cmd[j] = ft_strndup(line + start, i - start);
//         if (!cmd[j])
//         {
//             // a faire free avant ***
//             return (NULL);
//         }
//         j++;
//         start = i + 1;
//     }
//     i++;
// 	}
// 	cmd[j] = ft_strndup(line + start, i - start);
// 	if (!cmd[j])
// 	{
// 		// a faire free avant ***
// 		return (NULL);
// 	}
// 	cmd[j + 1] = NULL;
// 	return (cmd);
// }

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

// recuperer le nom de la variable d'env apres $
// a partir de l'index start (apres $) de str, on va recuperer le nom de la variable d'env
// ex) $USER -> start = index de U
char	*get_env_name(char *str, int start)
{
	int	i; // index pour parcourir str
	int	len; // la taille du nom de la variable d'env

	len = 0;
	i = start; // on va compter la taille a partir de l'index start
	if (!str)
		return (NULL);
	if (str[i] == '{') // s'il y a des accolades apres $
	{
		i++; // on passe le '{'
		while (str[i] && str[i] != '}') // on compte jusqu'a trouver '}'
		{
			i++;
			len++;
		}
		if (str[i] != '}') // s'il n'y a pas de '}' apres le nom de variable d'env, on return NULL
			return (NULL); // apres il faut gerer l'erreur *******
		if (len == 0) // ex) ${} , s'il y a pas de variable entre les accolades, on return NULL
			return (NULL);
		i--; // on revient a l'index du dernier caractere entre les accolades (avant '}')
		while (start < i) // pour verifier si le nom de variable entre {} est valide (alphabetique, chiffre, etc)
		{
			if (!ft_isalnum(str[i]) && str[i] != '_') // verifier si chaque caractere entre {} est valide
				return (NULL);
			i--; // on decremente pour verifier tout jusqu'au debut (apres '{')
		}
		return (ft_substr(str, start + 1, len)); 
		// le cas avec les accolades, on emmene a partir de start + 1 (pour passer le '{') et de taille len (avant '}')
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_')) 
	// le nom de variable peut etre soit l'alphabet soit le chiffre soit '_' (dans ce cas on continue pour compter la taille)
	{
		i++;
		len++;
	}
	if (len == 0) // s'il y a pas de variable apres $ , on return NULL
		return (NULL);
	return (ft_substr(str, start, len));
	// on retourne le nom de la variable d'env 
	// qui est a partir de l'index start et de taille len
}

// recuperer $ env variable
char	*get_env_var(char *str, char **env)
{
	int	j;
	int	len;

	j = 0;
	if (!str || !env)
		return (NULL);
	len = ft_strlen(str);
	while ((env)[j]) // on va parcourir tout env
	{
		if (ft_strncmp((env)[j], str, len) == 0 && env[j][len] == '=')
			return (env[j] + (len + 1)); // on retourne le contenu apres le '='
		j++;
	}
	return (NULL);
}

// ajouter un char c a la fin de la chaine resultat  
// ex) resultat = "youp", c = 'i' -> return "youpi"
char	*ajouter_char(char *resultat, char c)
{
	int		len; // la taille de resultat
	char	*temp; // temporaire pour stocker le nouveau resultat

	len = ft_strlen(resultat);
	temp = malloc(sizeof(char) * (len + 2)); // on agrandit de 2 pour le char c et le '\0'
	if (!temp)
		return (free(resultat), NULL);
	ft_strcpy(temp, resultat); // on copie resultat dans temp
	temp[len] = c; // on rajoute le char c passsé en parametre
	temp[len + 1] = '\0'; // et on termine avec 0
	free(resultat); // on libere l'ancien resultat
	return (temp);
}

// appliquer la variable d'env dans str a la position i (qui est le $)
//str = le str entier du token
//resultat = le nouveau str qui va etre créé
//i = la position du $ dans str (pour changer le variable d'origine, on utilise son pointeur)
//token = la structure principale qui contient env et exit status
// a faire : creer une nouvelle structure all (qui gere env, exit status) *************************************************************************
// puis changer t_token *token en t_mini *mini par exemple *************************************************************************
char	*appliquer_env_var(char *resultat, char *str, t_token *token, int *i)
{
	char	*env_name; // le nom de la variable d'env apres $ (ex. USER de $USER)
	char	*env_var; // la valeur de la variable d'env
	char	*temp; // temporaire pour stocker le nouveau resultat

	env_name = get_env_name(str, *i + 1); // on recupere le nom de la variable d'env apres $  (cf. *i + 1 <- la position apres $)
	// apres $, s'il y a un nom alphabetique ou chiffre ou '_', on le recupere
	// cf) il est possible de nom recupere ne soit pas dans env (ex. $qui_nexiste_pas)
	if (env_name) // si on a un nom de variable d'env
	{	
		env_var = get_env_var(env_name, token->env); // on recupere la valeur de la variable d'env
		if (env_var)
		{
			temp = ft_strjoin(resultat, env_var); // on concatene le resultat avec la valeur de la variable d'env
			if (!temp)
				return (free(env_name), free(resultat), NULL);
			free(resultat);
			resultat = temp; // on met a jour resultat
		}
		if (str[(*i) + 1] == '{') // si on a des accolades apres $ (ex. ${USER})
			*i = *i + ft_strlen(env_name) + 3; // on avance de la taille du nom + 3 (pour $ et les accolades)
			// cf) 2 pour les accolades + 1 pour $
		else // sinon (ex. $USER)
			*i = *i + ft_strlen(env_name) + 1; // +1 pour $
		free(env_name); // on libere env_name, puisqu'on l'a deja utilise
		return (resultat); // on retourne le resultat mis a jour
	}
	else // si on n'a pas de nom de variable d'env apres $  (ex. env_name == NULL ,  echo $)
		return (ajouter_char(resultat, str[(*i)++])); // on ajoute juste le caractere $ a resultat
		// (*i) : vu que i est le pointeur de int, pour recuperer la valeur, on fait *i
		// on applique la valeur str[(*i)] a la fonction ajouter_char
		// (*i)++ : puis, incrementer de 1 a la valeur (*i)  
}


// remplacement de $ par la valeur de la variable d'env
// parcours le token str et creer un nouveau str* result qui remplacera str;
// Si c’est un caractere $, on cherche la variable d’environment et on remplace
// sinon juste on copie caractere par caractere result est vide au depart et on le construit caractre par caractere
char	*remplacer_dollar(char *str, t_token *token)
{
	int		i; // l'index pour parcourir *str
	char	*resultat; // le nouveau str qui remplace *str

	resultat = malloc(sizeof(char) * 1); // on initialise resultat avec 1 caractere (pour resultat[0]='\0')
	if (!resultat)
		return (NULL);
	resultat[0] = '\0'; // resultat est vide au depart
	i = 0;
	while (str[i])
	{
		//si on voit $ (et pas $ a la fin de chaine)
		// on remplace par la valeur du nom et uniquement si on est pas dans des single quote
		if (str[i] == '$' && str[i + 1] != '\0')
			resultat = appliquer_env_var(resultat, str, token, &i); // on passe l'adresse de i pour le modifier dans la fonction
		// sinon on copie caractere par caractere str vers resultat
		else
			resultat = ajouter_char(resultat, str[i++]);
		if (!resultat)
			return (NULL);
	}
	return (resultat);
}


// int	appliquer_quote(t_token *token, char **env)
// {
// 	int		i;
// 	int		dollar_len;
// 	char	quote;
// 	char	*temp;
// 	char	*env_var;

// 	i = 0;
// 	dollar_len = 0;
// 	quote = 0;
// 	temp = NULL;
// 	env_var = NULL;
// 	if (!token || !token->str || !token->str[i])
// 		return (-1);
// 	while (token->str[i])
// 	{
// 		if ((token->str[i] == '"' || token->str[i] == '\'') && quote == 0)
// 		{
// 			quote = token->str[i];
// 			i++;
// 			continue ;
// 		}
// 		if (token->str[i] == quote && quote != 0)
// 		{
// 			quote = 0;
// 			i++;
// 			continue ;
// 		}
// 		if (token->str[i] == '$' && quote != '\'')
// 		{
// 			i++;
// 			dollar_len = len_dollar_espace(token->str + i);
// 			temp = strndup(token->str + i, (size_t)dollar_len);
// 			if (!temp)
// 				return (-1);
// 			env_var = get_env_var(temp, env);
// 			if (env_var != NULL)
// 				write(1, env_var, ft_strlen(env_var));
// 			i += dollar_len;
// 			free(temp);
// 			temp = NULL;
// 			continue ;
// 		}
// 		write(1, &token->str[i], 1);
// 		i++;
// 	}
// 	return (0);
// }


int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	char	*line;
	t_cmd	*cmd;
	t_token	*parsing;
	t_token	*temp;
	int		i;
	// t_minis	*mini;

	// mini = malloc(sizeof(mini));
	// if (!mini)
	// 	return (0);
	i = 0;
	cmd = NULL;
	while (1)
	{
		line = readline("coucou$ ");
		if (!line)
			break ;
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		parsing = NULL;
		i = 0;
		if (check_quotes(line) == 0)
		{
			printf("Error: unclosed quotes\n");
			free(line);
			continue ;
		}
		if (check_pipe_fin(line) == 1)
		{
			printf("Error: syntax error near unexpected token '|'\n");
			free(line);
			continue ;
		}
		parse_input(line, &parsing);
		temp = parsing;
		while (temp)
		{
			printf("noeud %d '%s' | type %s | type_quote %s\n", i, temp->str, get_token_type_str(temp->type_token), get_token_type_state(temp->type_quote));
			i++;
			temp = temp->next;
		}
		int result = decouper_cmd_par_pipe(parsing, &cmd);
		if (result == -1)
			return (-1);
		else if (result == -2)
		{
			continue ;
		}
		ft_print_cmds(cmd, count_pipe(parsing) + 1);

		free_tokens(&parsing);
		free(line);
	}
	return (0);
}
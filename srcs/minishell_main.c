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
	// printf("index fin: %d\n", i);
	i--; // quand on sort la boucle, c'est '\0'. donc on avance une fois
	while (i >= 0 && line[i] == ' ') // avancer s'il y a l'espace a la fin
		i--;
	// printf("index avant pipe ou char: %d\n", i);
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
		cmd[j].infile = NULL; // <
		cmd[j].outfile = NULL; // >
		cmd[j].temp_heredoc = NULL; // pour heredoc
		cmd[j].out_append = 0; // >>
		cmd[j].heredoc = 0; // <<
		cmd[j].limiter = NULL; // pour heredoc
		cmd[j].pid_heredoc = -1; // pid pour heredoc (fork)
		cmd[j].fd_in = -1;
		cmd[j].fd_out = -1;
		cmd[j].in_fail = 0;
		cmd[j].out_fail = 0;
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
int add_cmd(t_token *token, t_cmd *cmd/*, int nbr_cmd*/)
{
	int index_cmd; // l'index pour la structure  ex) tab[0] = {"echo", "hihi", NULL}, tab[1] = {"cat", "-e", NULL}
	int i; // l'index pour l'argument de chaque structure  ex) tab[0][0] = "echo", tab[0][1] = "hihi", tab[0][2] = NULL
	int	redir_existe;
	// printf("--- add_cmd ---\n");

	index_cmd = 0;
	i = 0;
	redir_existe = 0;
	// if (!cmd || nbr_cmd <= 0)
	// 	return (-1);
	while (token) // pendant que le noeud dans la liste chainee existe
	{
		if (token->type_token == T_MOT) // si le type de token est T_MOT
		{
			if (index_cmd < 0) // verifier l'index_cmd pour proteger
				return (-1);
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
		else if (token->type_token == T_FD_IN)
		{
			if (cmd[index_cmd].infile)
				free(cmd[index_cmd].infile); // s'il y a deja un infile, on le libere avant de le remplacer pour le nouveau
			// cf) cat < file1 < file2  -> on garde file2 seulement (donc, on libere file1 d'abord)
			cmd[index_cmd].infile = ft_strdup(token->str); // on ajoute le nom du fichier pour redirection <
		}
		else if (token->type_token == T_FD_OUT)
		{
			if (cmd[index_cmd].outfile)
				free(cmd[index_cmd].outfile);
			cmd[index_cmd].outfile = ft_strdup(token->str); // on ajoute le nom du fichier pour redirection >
			cmd[index_cmd].out_append = 0; // s'assurer que c'est pas en mode append
		}
		else if (token->type_token == T_FD_OUT_APPEND)
		{
			if (cmd[index_cmd].outfile)
				free(cmd[index_cmd].outfile);
			cmd[index_cmd].outfile = ft_strdup(token->str); // on ajoute le nom du fichier pour redirection >>
			cmd[index_cmd].out_append = 1; // marquer que c'est append (>>)
		}
		else if (token->type_token == T_FD_HEREDOC)
		{
			if (cmd[index_cmd].limiter)
				free(cmd[index_cmd].limiter);
			cmd[index_cmd].limiter = ft_strdup(token->str); // on ajoute le limiter pour heredoc (<<)  
			// ex) << EOF  -> limiter = "EOF"
			cmd[index_cmd].heredoc = 1; // marquer que c'est heredoc (<<)
		}
		if (token->type_token == T_PIPE) // si on arrive a '|'
		{
			if (cmd[index_cmd].infile || cmd[index_cmd].outfile || cmd[index_cmd].heredoc == 1)
				redir_existe = 1;
			else
				redir_existe = 0;
			if (token->next == NULL) // proteger au cas ou il y a un pipe a la fin (ex: cmd1 | )
				return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
			if (token->next->type_token == T_PIPE) // proteger au cas ou il y a 2 pipes consecutifs (ex: cmd1 || cmd2)
				return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
			if (cmd[index_cmd].cmd == NULL && i == 0 && !redir_existe)
			// proteger au cas ou il y a un pipe au debut (ex: | cmd1 )
				return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
			// if(cmd[index_cmd].cmd == NULL) // proteger au cas ou il y a 2 pipes consecutifs (ex: cmd1 || cmd2)
			// 	return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
			if (cmd[index_cmd].cmd != NULL) // si le tableau n'est pas vide 
			// ex) meme s'il y a pas d'argument T_MOT, mais il y a redir et fichier peut-etre
				cmd[index_cmd].cmd[i] = NULL; // on met le NULL terminateur pour cloturer argv
			index_cmd++; // on passe a la structure suivante
			i = 0; // pour reverifier des le debut, on reinitialise l'index pour l'argument de cette nouvelle structure
		}
		token = token->next; // on passe au noeud suivant
	}
	// printf("index_cmd final: %d\n", index_cmd);
	if (index_cmd > 0 && cmd[index_cmd].cmd == NULL
			&& cmd[index_cmd].infile == NULL && cmd[index_cmd].outfile == NULL && !cmd[index_cmd].heredoc)
			// proteger au cas ou il y a un pipe a la fin (ex: cmd1 | )
			// et aussi proteger le cas ou il y a un pipe avec redir mais sans mot (ex: cmd1 | < file)
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	// printf("index_cmd final2: %d\n", index_cmd);
	// printf("i final: %d\n", i);
	// if (cmd == NULL)
	// 	printf("probleme cmd");
	// else 
	// 	printf("pas de pb");
	// printf("cmd[0] %p", &cmd);
	if (cmd[index_cmd].cmd != NULL) // on termine aussi le dernier argv (apres la boucle)
		cmd[index_cmd].cmd[i] = NULL; // on ferme bien la fin 
	// printf("fin add_cmd\n");
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
		if (cmd[j].cmd == NULL)
			printf("cmd: NULL\n");
		else
		{
			while (cmd[j].cmd && cmd[j].cmd[i] != NULL)
			{
				printf("arg %d: %s\n", j, cmd[j].cmd[i]);
				i++;
			}
			j++;
			i = 0;
		}
	}
}


// On parse tout pour trouver les operations ou les builtins
// chaque noeud serait d'abord divise que par soit mot, soit redir, soit pipe  (cf. t_type token)
int parse_input(char *line, t_token **token, t_mini *mini) 
{
	// (void)mini;
	int						len;
	t_type_token	fd_type;

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
				add_token(line, T_RD_APPEND, 2, token); // on ajoute dans la liste chainee : >>, type : T_RD_APPEND;
				fd_type = T_FD_OUT_APPEND;
			}
			else if (!ft_strncmp(line, "<<", 2))
			{
				add_token(line, T_RD_HEREDOC, 2, token);
				fd_type = T_FD_HEREDOC;
			}
			line += 2;
		}
		else if (!ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1))
		{
			if (!ft_strncmp(line, ">", 1))
			{
				add_token(line, T_RD_OUT, 1, token);
				fd_type = T_FD_OUT;
			}
			else if (!ft_strncmp(line, "<", 1))
			{
				add_token(line, T_RD_IN, 1, token);
				fd_type = T_FD_IN;
			}
			line += 1;
		}
		else if (!ft_strncmp(line, "|", 1)) // pipe  (noeud '|'  /  type : T_PIPE)
		{
			if (fd_type != (t_type_token) - 1)
				return (write(2, "syntax error near unexpected token `|'\n", 40), -2);
			add_token(line, T_PIPE, 1, token);
			line += 1;
		}
		else
		{
			len = len_mot_total(line);
			if (fd_type != (t_type_token) - 1)
			{
				add_token(line, fd_type, len, token);
				fd_type = (t_type_token) - 1;
			}
			else
				add_token(line, T_MOT, len, token);
			line += len;
		}
	}
	if (fd_type != (t_type_token) - 1)
		return (write(2, "syntax error near unexpected token `newline'\n", 45), -2);
	if (appliquer_dollar_sur_liste_token(token, mini) == -1)
		return (-1);
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
	if (str[i] == '?') // cas special pour $?
		return (ft_substr(str, start, 1));
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
char	*get_env_var(char *str, t_mini *mini)
{
	int	j;
	int	len;

	j = 0;
	if (!str || !mini || !mini->env)
		return (NULL);
	if (str[0] == '?' && str[1] == '\0') // cas special pour $? (convertir le code de sortie en chaîne)
		return (ft_itoa(mini->exit_status)); // pour convertir exit status en str
		// exit status est un int, donc on utilise ft_itoa pour le convertir en str, pour afficher dans notre minishell
		// vu qu'on a alloue le memoire dans ft_itoa, il faut liberer apres l'utilisation ************
	len = ft_strlen(str);
	while ((mini->env)[j]) // on va parcourir tout env
	{
		if (ft_strncmp((mini->env)[j], str, len) == 0 && (mini->env)[j][len] == '=')
			return (ft_strdup((mini->env)[j] + (len + 1))); // on retourne le contenu apres le '=' (free apres *****)
		j++;
	}
	return (ft_strdup("")); // si on ne trouve pas la variable d'env, on retourne une chaine vide
	// penser a free apres **************
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
// str = le str entier du token
// resultat = le nouveau str qui va etre créé
// i = la position du $ dans str (pour changer le variable d'origine, on utilise son pointeur)
// mini = la structure principale qui contient env et exit status
char	*appliquer_env_var(char *resultat, char *str, t_mini *mini, int *i)
{
	char	*env_name; // le nom de la variable d'env apres $ (ex. USER de $USER)
	char	*env_var; // la valeur de la variable d'env
	char	*temp; // temporaire pour stocker le nouveau resultat

	env_name = get_env_name(str, *i + 1); // on recupere le nom de la variable d'env apres $  (cf. *i + 1 <- la position apres $)
	// apres $, s'il y a un nom alphabetique ou chiffre ou '_', on le recupere
	// cf) il est possible de nom recupere ne soit pas dans env (ex. $qui_nexiste_pas)
	if (env_name) // si on a un nom de variable d'env
	{	
		env_var = get_env_var(env_name, mini); // on recupere la valeur de la variable d'env
		if (!env_var)
			return (free(env_name), free(resultat), NULL);
		temp = ft_strjoin(resultat, env_var); // on concatene le resultat avec la valeur de la variable d'env
		if (!temp)
			return (free(env_var), free(env_name), free(resultat), NULL);
		free(env_var); // on libere env_var, puisqu'on l'a deja utilise
		free(resultat); // on libere l'ancien resultat
		resultat = temp; // on met a jour resultat
		// if (str[(*i) + 1] == '{') // si on a des accolades apres $ (ex. ${USER})
		// 	- *i = *i + ft_strlen(env_name) + 3; // on avance de la taille du nom + 3 (pour $ et les accolades)
			// cf) 2 pour les accolades + 1 pour $
		// else // sinon (ex. $USER)
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
// le cas de single quote a gerer aussi (ne pas remplacer $ dans des single quote) *******************************
char	*remplacer_dollar(char *str, t_mini *mini)
{
	int		i; // l'index pour parcourir *str
	int		s_quote;
	int		d_quote;
	char	*resultat; // le nouveau str qui remplace *str

	resultat = malloc(sizeof(char) * 1); // on initialise resultat avec 1 caractere (pour resultat[0]='\0')
	if (!resultat)
		return (NULL);
	resultat[0] = '\0'; // resultat est vide au depart
	i = 0;
	s_quote = 0; // pour gerer le cas de single quote (0 = pas dans single quote, 1 = dans single quote)
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote) // gerer le cas de single quote au debut
		{
			s_quote = !s_quote; // on inverse l'etat de s_quote (0 -> 1 , 1 -> 0)
			resultat = ajouter_char(resultat, str[i++]); // on ajoute le caractere quote a resultat
			if (!resultat)
				return (NULL);
			continue ; // on passe au caractere suivant
		}
		if (str[i] == '"' && !s_quote) // gerer le cas de double quote au debut
		{
			d_quote = !d_quote; // on inverse l'etat de d_quote (0 -> 1 , 1 -> 0)
			resultat = ajouter_char(resultat, str[i++]); // on ajoute le caractere quote a resultat
			if (!resultat)
				return (NULL);
			continue ; // on passe au caractere suivant
		}
		//si on voit $ (et pas $ a la fin de chaine)
		// on remplace par la valeur du nom et uniquement si on est pas dans des single quote
		if ((str[i] == '$' && str[i + 1] != '\0') && !s_quote)
			resultat = appliquer_env_var(resultat, str, mini, &i); // on passe l'adresse de i pour le modifier dans la fonction
		// sinon on copie caractere par caractere str vers resultat
		else
			resultat = ajouter_char(resultat, str[i++]);
		if (!resultat)
			return (NULL);
	}
	return (resultat);
}

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
		if (temp->type_token == T_MOT) // si le type de token est T_MOT
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



// ===================================== redirection ===================================== 
// ======================================================================================= 

// appliquer la redirection outfile (>) pour la commande i
int	appliquer_outfile(t_mini *mini, int i)
{
	if (mini->cmd[i].out_fail || mini->cmd[i].in_fail) // si deja echec de redir in ou out, on ne fait rien
		return (0);
	if (mini->cmd[i].outfile == NULL) // proteger au cas ou outfile est NULL
	{
		mini->exit_status = 2;
		return (-1);
	}
	if (mini->cmd[i].fd_out != -1) // si fd_out est deja ouvert, on le ferme d'abord
	{
		close(mini->cmd[i].fd_out);
		mini->cmd[i].fd_out = -1; // reinitialiser fd_out
	}
	mini->cmd[i].fd_out = open(mini->cmd[i].outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	// ouvrir le fichier en ecriture, tronquer s'il existe, creer s'il n'existe pas
	if (mini->cmd[i].fd_out < 0) // si echec d'ouverture
	{
		if (mini->cmd[i].out_fail == 0 && mini->cmd[i].in_fail == 0) // pour ne pas afficher plusieurs fois l'erreur
			perror(mini->cmd[i].outfile); // afficher l'erreur
		mini->exit_status = 1; // mettre le code de sortie a 1
		mini->cmd[i].fd_out = -1; // marquer que l'ouverture a echoue
		mini->cmd[i].out_fail = 1; // marquer que l'ouverture a echoue
	}
	return (0);
}

// appliquer la redirection outfile (>>) pour la commande i
int	appliquer_append(t_mini *mini, int i)
{
	if (mini->cmd[i].out_fail || mini->cmd[i].in_fail) // si deja echec de redir in ou out, on ne fait rien
		return (0);
	if (mini->cmd[i].outfile == NULL) // proteger au cas ou outfile est NULL
	{
		mini->exit_status = 2;
		return (-1);
	}
	if (mini->cmd[i].fd_out != -1) // si fd_out est deja ouvert, on le ferme d'abord
	{
		close(mini->cmd[i].fd_out);
		mini->cmd[i].fd_out = -1; // reinitialiser fd_out
	}
	mini->cmd[i].fd_out = open(mini->cmd[i].outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	// ouvrir le fichier en ecriture, ajouter a la fin s'il existe, creer s'il n'existe pas
	if (mini->cmd[i].fd_out < 0) // si echec d'ouverture
	{
		if (mini->cmd[i].out_fail == 0 && mini->cmd[i].in_fail == 0) // pour ne pas afficher plusieurs fois l'erreur
			perror(mini->cmd[i].outfile); // afficher l'erreur
		mini->exit_status = 1; // mettre le code de sortie a 1
		mini->cmd[i].fd_out = -1; // marquer que l'ouverture a echoue
		mini->cmd[i].out_fail = 1; // marquer que l'ouverture a echoue
	}
	return (0);
}

// Passe la structure globale et l'index de la commande en argument
// et applique la redirection de sortie en fonction du type (>, >>)
void	process_out_redir(t_mini *mini, int i)
{
	if (!mini || i < 0 || i >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return ;
	if (!mini->cmd || !mini->cmd[i].outfile) // si cmd n'existe pas ou outfile est NULL
		return ;
	if (mini->cmd[i].out_fail || mini->cmd[i].in_fail) // si deja echec de redir in ou out, on ne fait rien
		return ;
	if (!mini->cmd[i].out_append) // si out_append == 0, c'est une redirection simple (>)
		appliquer_outfile(mini, i);
	else if (mini->cmd[i].out_append == 1) // si out_append == 1, c'est une redirection en mode append (>>)
		appliquer_append(mini, i);
}

// appliquer la redirection infile (<) pour la commande i
int	appliquer_infile(t_mini *mini, int i)
{
	if (mini->cmd[i].in_fail || mini->cmd[i].out_fail) // si deja echec de redir in ou out, on ne fait rien
		return (0);
	if (mini->cmd[i].infile == NULL) // proteger au cas ou infile est NULL
	{
		mini->exit_status = 2;
		return (-1);
	}
	if (mini->cmd[i].fd_in != -1) // si fd_in est deja ouvert, on le ferme d'abord
	{
		close(mini->cmd[i].fd_in);
		mini->cmd[i].fd_in = -1; // reinitialiser fd_in
	}
	mini->cmd[i].fd_in = open(mini->cmd[i].infile, O_RDONLY); // ouvrir le fichier en lecture seule
	if (mini->cmd[i].fd_in < 0) // si echec d'ouverture
	{
		perror(mini->cmd[i].infile); // afficher l'erreur
		mini->exit_status = 1; // mettre le code de sortie a 1
		mini->cmd[i].fd_in = -1; // marquer que l'ouverture a echoue
		mini->cmd[i].in_fail = 1; // marquer que l'ouverture a echoue
	}
	return (0);
}


// Préparation du fichier temporaire pour heredoc
void	preparer_temp_file(t_mini *mini, int i)
{
	char	*temp_index; // pour faire l'etiquette de l'index du fichier temporaire

	if (mini->cmd[i].fd_in != -1) // si fd_in est deja ouvert, on le ferme d'abord
		close (mini->cmd[i].fd_in); // fermer l'ancien descripteur de fichier
	if (mini->cmd[i].temp_heredoc)
	{
		unlink(mini->cmd[i].temp_heredoc); // supprimer l'ancien fichier temporaire s'il existe
		free(mini->cmd[i].temp_heredoc); // free l'ancien nom de fichier temporaire
		mini->cmd[i].temp_heredoc = NULL; // reinitialiser a NULL
	}
	temp_index = ft_itoa(i); // convertir l'index i en string pour nommer l'index du fichier temporaire
	if (!temp_index)
	{
		mini->cmd[i].in_fail = 1;
		perror("malloc: temp index");
		return ;
	}
	mini->cmd[i].temp_heredoc = ft_strjoin("temp_", temp_index); // nom du fichier temporaire pour heredoc"
	free(temp_index); // liberer temp_index apres utilisation
	if (!mini->cmd[i].temp_heredoc) // si echec de malloc pour le nom du fichier temporaire
	{
		mini->cmd[i].in_fail = 1;
		perror("malloc: temp heredoc name");
		return ;
	}
	if (access(mini->cmd[i].temp_heredoc, F_OK) == 0) // si le fichier mini->cmd[i].temp_heredoc existe deja
		unlink(mini->cmd[i].temp_heredoc); // supprimer le fichier existant
	mini->cmd[i].fd_in = open(mini->cmd[i].temp_heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// ouvrir (créer) le fichier temporaire en écriture
	if (mini->cmd[i].fd_in == -1)
	{
		mini->cmd[i].in_fail = 1;
		perror("open temp");
	}
}

// afficher le message d'erreur quand on saisit ctrl d dans heredoc
void	print_heredoc_warning_ctrl_d(char *delimiter)
{
	if (!delimiter)
		delimiter = "";
	write(2, "warning: here-document delimited by end-of-file (wanted `", 57);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
	// le nombre de lines a faire apres ******************************************************
}

// recuperer les lignes de heredoc, puis les stocker dans le fichier temp
// fd = fd de fichier temporaire temp, delimiter = limiter
int	collecter_heredoc_lines(int fd, char *delimiter)
{
	char *line;
	
	while (1)
	{
		line = readline("> "); // afficher un prompte qui ressemble a heredoc
		if (!line) // saisit ctrl+D -> on quitte
			return (1); // dans ce cas (ctrl+D), pas besoin de liberer le memoire, puisqu'il y en a pas
		if (delimiter && strcmp(line, delimiter) == 0) // quand on croise limiter -> on quitte
		// si delimiter est NULL -> erreur (pour proteger on ajoute dans la condition delimiter aussi)
		{
			// if (!line) // ctrl d message d'erreur a faire apres *************************
			// 	return;
			free(line); // liberer readline
			return (0); // quitte la boucle (et cette fonction)
		}
		write(fd, line, strlen(line)); // le resultat
		write(fd, "\n", 1); // vu que readline n'applique pas automatiquement '\n', on en ajoute a la fin
		free(line); // free readline avant de quitter la fonction hihi
	}
	return (0);
}

// appliquer heredoc dans le processus enfant
void	appliquer_heredoc_enfant(t_mini *mini, int i)
{
	int	resultat;

	// il faut gerer des signaux; plus tard *******************************************
	signal(SIGINT, SIG_DFL); // quand on saisit ctrl+C, le processus enfant doit etre termine
	signal(SIGQUIT, SIG_IGN); // quand on saisit ctrt+\, on l'ignore (ca change rien)
	preparer_temp_file(mini, i); // creer un fichier temporaire
	if (mini->cmd[i].fd_in == -1)
		exit (1);
	resultat = collecter_heredoc_lines(mini->cmd[i].fd_in, mini->cmd[i].limiter);
	// collecter des lignes heredoc dans le fichier temp
	if (resultat == 1) // ctrl-D
	{
		print_heredoc_warning_ctrl_d(mini->cmd[i].limiter);
		close(mini->cmd[i].fd_in);
		exit (0);
	}
	close(mini->cmd[i].fd_in); //close dans l'enfant pour eviter les leak
	exit(0);
}

// gerer au cas de ctrl-C
void	appliquer_sigint_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

// gerer les sigaux (ctrl-C, ctrl-\)
void	init_signaux(void)
{
	signal(SIGINT, appliquer_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

// appliquer heredoc pour la commande i
int	appliquer_heredoc_cmd(t_mini *mini, int i)
{
	int	status; // pour waitpid (si le processus enfant s'est termine correctement)
	int	exit_status; // pour resultat du waitpid (code de sortie du processus enfant)
	int	exit_signal;

	signal(SIGINT, SIG_IGN); // le processus parent ignore ctrl+C pendant le processus enfant (heredoc fork)
	signal(SIGQUIT, SIG_IGN);
	mini->cmd[i].pid_heredoc = fork(); // creer un processus enfant pour gerer heredoc
	if (mini->cmd[i].pid_heredoc == -1) // si echec de fork
		return (-1);
	if (mini->cmd[i].pid_heredoc == 0) // processus enfant
		appliquer_heredoc_enfant(mini, i);
	if (waitpid(mini->cmd[i].pid_heredoc, &status, 0) == -1) // attendre la fin du processus enfant
		return (init_signaux(), -1); // si echec de waitpid
	init_signaux(); // apres la fin du processus enfant, on applique des signaux pareils que shell
	if (WIFSIGNALED(status))
	{
		exit_signal = WTERMSIG(status);
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 128 + exit_signal;
		return (-1);
	}
	if (WIFEXITED(status)) // si le processus enfant s'est termine correctement
	{
		exit_status = WEXITSTATUS(status); // recuperer le code de sortie
		if (exit_status != 0) 
		// si le code de sortie n'est pas 0, ni 2 (erreur dans heredoc)
		{
			mini->cmd[i].in_fail = 1; // marquer l'echec de heredoc
			mini->exit_status = exit_status; // mettre a jour le code de sortie global
			return (-1); // retourner -1 pour indiquer l'erreur
		}
	}
	else // si le processus enfant ne s'est pas termine correctement
	{
		mini->cmd[i].in_fail = 1; // marquer l'echec de heredoc
		mini->exit_status = 1; // mettre a jour le code de sortie global
		return (-1);
	}
	if (!mini->cmd[i].temp_heredoc) // proteger au cas ou temp_heredoc est NULL
	{
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 1;
		return (-1);
	}
	if (mini->cmd[i].fd_in != -1) // si fd_in est deja ouvert, on le ferme d'abord
		close(mini->cmd[i].fd_in); // fermer l'ancien fd_in avant de le remplacer
	mini->cmd[i].fd_in = open(mini->cmd[i].temp_heredoc, O_RDONLY); // reouvrir le fichier temp en lecture seule
	if (mini->cmd[i].fd_in == -1) // si echec d'ouverture de temp en lecture
	{
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 1;
		perror("open temp for reading"); // afficher l'erreur
		return (-1);
	}
	return (0);
}


int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	// (void)env;
	char	*line;
	t_cmd	*cmd;
	t_token	*parsing;
	t_token	*temp;
	int		i;
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (0);
	mini->env = env;
	mini->exit_status = 0;
	mini->cmd = NULL;
	mini->nbr_cmd = 0;
	i = 0;
	cmd = NULL;
	while (1)
	{
		init_signaux();
		line = readline("coucou$ ");
		if (!line) // ctrl D
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		// printf("Input line: %s\n", line);
		add_history(line);

		parsing = NULL;
		i = 0;
		if (check_quotes(line) == 0)
		{
			printf("Error: unclosed quotes\n");
			free(line);
			continue ;
		}
		// printf("Quotes are properly closed.\n");
		if (check_pipe_fin(line) == 1)
		{
			printf("Error: syntax error near unexpected token '|'\n");
			free(line);
			continue ;
		}
		// printf("Pipe at the end is properly handled.\n");
		// printf("----- Parsing tokens -----\n");
		parse_input(line, &parsing, mini);
		
		// printf("----- parse_input applique -----\n");
		temp = parsing;
		while (temp)
		{
			printf("testing tokens:\n");
			printf("noeud %d '%s' | type %s | type_quote %s\n", i, temp->str, get_token_type_str(temp->type_token), get_token_type_state(temp->type_quote));
			i++;
			temp = temp->next;
		}
		// printf("-----1wejwej---------------------\n");
		cmd = malloc_cmd(parsing);
		int result = add_cmd(parsing, cmd);
		if (result == -1)
			return (-1);
		else if (result == -2)
		{
			continue ;
		}
		mini->cmd = cmd;
		mini->nbr_cmd = count_pipe(parsing) + 1;

		// printf("-----2wejwej---------------------\n");
		test_print_cmds(cmd, count_pipe(parsing) + 1);
		if (parse_input(line, &parsing, mini) == -1)
		{
			printf("Error: parse_input failed\n");
			free_tokens(&parsing);
			free(line);
			continue ;
		}
		// free cmd a gerer ******************************
		if (cmd)
			free(cmd); // il faut ameliorer apres *************************
		mini->cmd = NULL;
		mini->nbr_cmd = 0;
		free_tokens(&parsing);
		free(line);
	}
	return (0);
}

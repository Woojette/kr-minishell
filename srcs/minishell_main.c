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
	else if (line[0] != '"' && line[0] != '\'')
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
	int		count;
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

		// redirections: tableaux (on garde tous les noms de fichiers dans l'ordre)
		cmd[j].infile = NULL; // liste des fichiers pour  < (char **, termine par NULL)
		cmd[j].outfile = NULL; // liste des fichiers pour > et >> (char **, termine par NULL)

		cmd[j].temp_heredoc = NULL; // tableau (char **) des noms de fichiers temporaire pour heredoc (qui termine par NULL)
		cmd[j].limiter = NULL; // tableau (char **) des limiters pour heredoc (qui termine par NULL)
		cmd[j].compter_heredoc = 0; // pour compter le nombre de heredoc (<<) dans chaque commande

		cmd[j].in_heredoc = NULL; // tableau int pour sauvegarder le type de in redir (<, <<) par ordre
		cmd[j].in_hd_index = NULL; // index de chaque infile(<) et limiter(<<) qui concerne chaque redir de in_heredoc
		cmd[j].compter_in_hd = 0; // compter le nombre de in redir d'entree (<, <<)

		cmd[j].hd_env = NULL; // tableau int: appliquer env expansion pour chaque limiter[n] (1 env, 0 sinon)

		// out_append[j] correspond a outfile[j]: 1 pour >> (append), 0 pour > (truncate)
		cmd[j].out_append = NULL; // initialiser a NULL pour proteger
		// NULL au debut: on alloue et agrandit au fur et a mesure dans add_cmd()
		// cf) out_append[n] = 1 pour >> (append),  0 pour > (truncate)

		// cmd[j].temp_heredoc = NULL; // pour heredoc
		cmd[j].heredoc = 0; // <<
		// cmd[j].limiter = NULL; // pour heredoc
		cmd[j].pid_heredoc = -1; // pid pour heredoc (fork)
		cmd[j].fd_in = -1;
		cmd[j].fd_out = -1;
		cmd[j].in_fail = 0;
		cmd[j].out_fail = 0;
		j++;
	}
	return (cmd);
}

// compter le nombre de chaines dans un double tableau char**
int	len_tab_char(char **tab)
{
	int	j;

	j = 0;
	if (!tab)
		return (0);
	while (tab[j])
		j++;
	return (j);
}

// // compter le nombre d'elements dans un tableau int*
// int	len_tab_int(int *tab)
// {
// 	int	j;

// 	j = 0;
// 	if (!tab)
// 		return (0);
// 	while (tab[j] != NULL) // on utilise -1 pour marquer la fin du tableau int
// 		j++;
// 	return (j);
// }

// fonction pour agrandir un tableau et rajouter une valeur int (1 ou 0), comme pour out_append
int	*add_double_tab_int(int *tab, int val, int size)
{
	int *new_tab; // nouveau tableau agrandi
	int j; // index pour parcourir les tableaux
	// int size; // taille actuelle du tableau int

	// size = len_tab_char(tab); // compter le nombre d'elements dans le tableau char
	if (!tab && size > 0) // proteger au cas ou tab est NULL mais size > 0
		return (NULL);
	if (size < 0)
		return (NULL);
	new_tab = malloc(sizeof(int) * (size + 1)); // +1 pour le nouveau 
	// cf) int *tab n'a pas besoin d'ajouter '\0' a la fin 
	if (!new_tab)
			return (NULL);
	j = 0;
	while (j < size)
	{
			new_tab[j] = tab[j]; // copier chaque valeur
			j++;
	}
	new_tab[j] = val; // ajouter la nouvelle valeur (1 ou 0)
	free(tab); // vu qu'on a bien cree un nouveau tableau agrandi, on libere l'ancien tableau
	return (new_tab);
}

// fonction pour agrandir un tableau et rajouter une chaine, comme pour le builtin export
// char **tab : double tableau actuel, *str : nouveau tableau a ajouter,
// int  size : taille actuelle de double tableau
char** add_double_tab_char(char **tab, char *str, int size)
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
				// cf) on copie juste le pointeur (l'adresse)
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
	int 	index_cmd; // l'index pour la structure  ex) tab[0] = {"echo", "hihi", NULL}, tab[1] = {"cat", "-e", NULL}
	int 	i; // l'index pour l'argument de chaque structure  ex) tab[0][0] = "echo", tab[0][1] = "hihi", tab[0][2] = NULL
	int		n; // l'index pour limiters de heredoc
	int		redir_existe;
	char	*mot_temp; // temporaire pour le mot
	char	*file_temp; // temporaire pour le nom de fichier
	int		size_file_tab; // pour compter la taille actuelle du tableau de fichiers (infile ou outfile) pour agrandir le tableau et ajouter un nouveau fichier
	char	*limiter_sans_quote;
	int		limiter_env;

	index_cmd = 0;
	i = 0;
	n = 0;
	redir_existe = 0;
	mot_temp = NULL;
	file_temp = NULL;
	size_file_tab = 0;
	limiter_sans_quote = 0;
	limiter_env = 0;
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
				cmd[index_cmd].cmd[0] = ft_strdup(token->str); // on ajoute le contenu de token a cet argument de la telle structure
				if (!cmd[index_cmd].cmd[0])
					return (-1);
				cmd[index_cmd].cmd[1] = NULL; // vu que c'est un double tableau (tableau de pointeurs char *, donc argv), on place d'abord le NULL final
			}
			else // s'il y a deja un argument dans le double tableau, on agrandit le tableau (pour ajouter un nouvel arguement)
			{
				mot_temp = ft_strdup(token->str); // dupliquer le contenu de token->str (pour proteger)
				if (!mot_temp)
					return (-1);
				cmd[index_cmd].cmd = add_double_tab_char(cmd[index_cmd].cmd, mot_temp, i); // agrandir le tableau cmd[index_cmd].cmd pour ajouter le nouveau mot
				// ex) tab[0] = {"echo", NULL} -> {"echo", "hihi", NULL} 
				if (!cmd[index_cmd].cmd)
					return (-1);
			}
			i++; // i = nombre d'arguments actuels (prochain index libre)
		}
		else if (token->type_token == T_FD_IN)
		{
			file_temp = ft_strdup(token->str); // dupliquer le nom du fichier (redirection <) pour stocker dans cmd
			if (!file_temp)
				return (-1);
			size_file_tab = len_tab_char(cmd[index_cmd].infile); // compter la taille actuelle du tableau infile
			cmd[index_cmd].infile = add_double_tab_char(cmd[index_cmd].infile, file_temp, size_file_tab);
			// agrandir le tableau infile pour ajouter le nouveau fichier
			// ex) cat < file1 < file2  -> infile = {"file1", "file2", NULL}
			// on n'ecrase plus, on stocke tout dans l'ordre
			if (!cmd[index_cmd].infile)
				return (-1);
			cmd[index_cmd].in_heredoc = add_double_tab_int(cmd[index_cmd].in_heredoc, 0, cmd[index_cmd].compter_in_hd);
			// ajouter le type de in redir: dans ce cas c'est in (<), on ajoute 0
			if (!cmd[index_cmd].in_heredoc)
				return (-1);
			cmd[index_cmd].in_hd_index = add_double_tab_int(cmd[index_cmd].in_hd_index, size_file_tab, cmd[index_cmd].compter_in_hd);
			// sauvegarde l'index de infile de cette in redir
			if (!cmd[index_cmd].in_hd_index)
				return (-1);
			cmd[index_cmd].compter_in_hd++; // des qu'on ajoute in redir, on incremente de 1 (l'index de in redir)
		}
		else if (token->type_token == T_FD_OUT)
		{
			file_temp = ft_strdup(token->str); // dupliquer le nom du fichier (redirection >) pour stocker dans cmd
			if (!file_temp)
				return (-1);
			size_file_tab = len_tab_char(cmd[index_cmd].outfile); // compter la taille actuelle du tableau outfile
			cmd[index_cmd].outfile = add_double_tab_char(cmd[index_cmd].outfile, file_temp, size_file_tab); // agrandir le tableau outfile pour ajouter le nouveau fichier
			if (!cmd[index_cmd].outfile)
				return (-1);
			cmd[index_cmd].out_append = add_double_tab_int(cmd[index_cmd].out_append, 0, size_file_tab);
			// agrandir le tableau out_append pour ajouter 0 (truncate) <- puisqu'on est dans la condition T_FD_OUT >
			// 0='>' (truncate), 1='>>' (append)
			if (!cmd[index_cmd].out_append)
				return (-1);
		}
		else if (token->type_token == T_FD_OUT_APPEND)
		{
			file_temp = ft_strdup(token->str); // dupliquer le nom du fichier (redirection >>) pour stocker dans cmd
			if (!file_temp)
				return (-1);
			size_file_tab = len_tab_char(cmd[index_cmd].outfile); // compter la taille actuelle du tableau outfile
			cmd[index_cmd].outfile = add_double_tab_char(cmd[index_cmd].outfile, file_temp, size_file_tab); // agrandir le tableau outfile pour ajouter le nouveau fichier
			if (!cmd[index_cmd].outfile)
				return (-1);
			cmd[index_cmd].out_append = add_double_tab_int(cmd[index_cmd].out_append, 1, size_file_tab); // agrandir le tableau out_append pour ajouter 1 (append) <- puisqu'on est dans la condition T_FD_OUT_APPEND >>
			// 0='>' (truncate), 1='>>' (append)
			if (!cmd[index_cmd].out_append)
				return (-1);
		}
		else if (token->type_token == T_FD_HEREDOC)
		{
			n = cmd[index_cmd].compter_heredoc; // n = nombre de heredoc deja existant pour cette commande

			limiter_env = check_heredoc_env(token->str);
			// memoriser pour chaque heredoc si l'expansion de l'env doit etre faite ou non
			if (check_quote_limiter(token->str)) // enlever des quotos s'il y en a
				limiter_sans_quote = enlever_quote_dans_token(token->str);
			else
				limiter_sans_quote = ft_strdup(token->str);
			if (!limiter_sans_quote)
				return (-1);

			cmd[index_cmd].limiter = add_double_tab_char(cmd[index_cmd].limiter, limiter_sans_quote, n); 
			// sauvegarder et agrandir le tableau limiter (sans quote) pour ajouter le nouveau limiter
			if (!cmd[index_cmd].limiter)
				return (-1);

			cmd[index_cmd].hd_env = add_double_tab_int(cmd[index_cmd].hd_env, limiter_env, n);
			// sauvegarder le type de l'expansion de l'env de chaque cas
			if (!cmd[index_cmd].hd_env)
				return (-1);

			cmd[index_cmd].compter_heredoc++; // incrementer le nombre de heredoc pour cette commande
			cmd[index_cmd].heredoc = 1; // marquer que c'est heredoc (<<)
			cmd[index_cmd].in_heredoc = add_double_tab_int(cmd[index_cmd].in_heredoc, 1, cmd[index_cmd].compter_in_hd);
			// ajouter le type de in redir: dans ce cas c'est heredoc (<<), on ajoute 1
			if (!cmd[index_cmd].in_heredoc)
				return (-1);
			cmd[index_cmd].in_hd_index = add_double_tab_int(cmd[index_cmd].in_hd_index, n, cmd[index_cmd].compter_in_hd);
			// sauvegarde l'index de infile de cette heredoc redir
			if (!cmd[index_cmd].in_hd_index)
				return (-1);
			cmd[index_cmd].compter_in_hd++; // des qu'on ajoute in redir, on incremente de 1 (l'index de in redir)
		}
		if (token->type_token == T_PIPE) // si on arrive a '|'
		{
			if (cmd[index_cmd].infile || cmd[index_cmd].outfile || cmd[index_cmd].compter_in_hd > 0)
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
	int				len;
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
	if (appliquer_dollar_sur_liste_token(token, mini) == -1) // appliquer dollar en respectant a quotes
		return (-1);
	if (appliquer_quote_sur_liste_token(token) == -1) // apres l'expansion de dollar, on supprime quote
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
		if (temp->type_token == T_MOT || temp->type_token == T_FD_IN
			|| temp->type_token == T_FD_OUT || temp->type_token == T_FD_OUT_APPEND)
			// si le type de token est T_MOT et redir -> on applique le remplacement de $
			// heredoc sera gere ailleurs
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

// enlever les quotes dans un token str
char	*enlever_quote_dans_token(char *str)
{
	int	i; // index pour parcourir str
	int	n; // index pour parcourir resultat (nouveau str sans quotes)
	int	s_quote; // entree dans single quote ou pas (1 = dans single quote, 0 sinon)
	int	d_quote; // entree dans double quote ou pas (1 = dans double quote, 0 sinon)
	char	*resultat; // le nouveau str sans quotes

	i = 0;
	n = 0;
	s_quote = 0; // pour gerer le cas de dans single quote (1 = dans single quote, 0 sinon)
	d_quote = 0; // pour gerer le cas de double quote (1 = dans double quote, 0 sinon)
	if (!str)
		return (NULL);
	resultat = malloc(sizeof(char) * (ft_strlen(str) + 1)); // resultat ne peut pas etre plus long que str (puisqu'on va juste enlever les quotes)
	// donc on alloue la taille de str + 1 pour le '\0'
	if (!resultat)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote) // gerer le cas de single quote au debut
		{
			s_quote = !s_quote; // on inverse l'etat de s_quote (0 -> 1 , 1 -> 0)
			i++; // on saute le caractere quote
			continue ; // on passe au caractere suivant
		}
		if (str[i] == '"' && !s_quote) // gerer le cas de double quote au debut
		{
			d_quote = !d_quote; // on inverse l'etat de d_quote (0 -> 1 , 1 -> 0)
			i++; // on saute le caractere quote
			continue ; // on passe au caractere suivant
		}
		resultat[n++] = str[i++]; // on copie le caractere dans resultat et on avance les index
		// c'est pareil que resulat[n] = str[i]; n++; i++; hihi j'ai appris
		// ex) you"pi'i'i" -> youpii
	}
	resultat[n] = '\0'; // terminer resultat par '\0'
	if (s_quote || d_quote) // si on a une quote non fermee, on retourne NULL (erreur de syntaxe)
		return (free(resultat), NULL);
	return (resultat); // retourner le nouveau str sans quotes
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
			|| temp->type_token == T_FD_HEREDOC))
			// on enleve les quotes si le type de token est T_MOT ou redir
		{
			// printf("avant enlever quote: [%s]\n", temp->str);
			new_str = enlever_quote_dans_token(temp->str); // enlever les quotes dans token->str
			if (!new_str)
				return (free_tokens(token), -1);
			free(temp->str);
			temp->str = new_str; // on met a jour token->str avec le nouveau str
			// printf("apres enlever quote: [%s]\n", temp->str);
		}
		temp = temp->next; // passer au noeud suivant
	}
	return (0);
}




// ===================================== redirection ===================================== 
// ======================================================================================= 



// Passe la structure globale et l'index de la commande en argument
// et applique la redirection de sortie en fonction du type (>, >>)
void	process_out_redir(t_mini *mini, int i)
{
	int	n; // index pour parcourir outfile
	int	type_redir; // type de redirection pour le fichier (1 = append, 0 = truncate)
	int	fd_temp; // fd temporaire pour ouvrir le fichier par mesure de precaution

	n = 0;
	type_redir = 0; // > par defaut
	fd_temp = -1;
	if (!mini || i < 0 || i >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return ;
	if (!mini->cmd || !mini->cmd[i].outfile) // si cmd n'existe pas ou outfile est NULL
		return ;
	if (mini->cmd[i].out_fail || mini->cmd[i].in_fail) // si deja echec de redir in ou out, on ne fait rien
		return ;
	while (mini->cmd[i].outfile[n]) // pour chaque fichier de redirection outfile
	{
		type_redir = 0; // a chaque iteration de outfile[n], type_redir commence toujours par 0 (> par defaut)
		fd_temp = -1; // on initialise a nouveau le fd temporaire a chaque iteration
		// si un tableau out_append existe, on recupere le type de redir associe a ce fichier
		// out_append[n] correspond a outfile[n] (0 est >, 1 est >>)
		if (!mini->cmd[i].out_append)
			type_redir = 0; // s'il y a un probleme, 0 par defaut
		else
			type_redir = mini->cmd[i].out_append[n];
		if (type_redir == 1) // si out_append == 1, c'est une redirection en mode append (>>)
			fd_temp = open(mini->cmd[i].outfile[n], O_WRONLY | O_APPEND | O_CREAT, 0644);
		else // si out_append == 0, c'est une redirection simple (>)
			fd_temp = open(mini->cmd[i].outfile[n], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd_temp < 0)
		{
			perror(mini->cmd[i].outfile[n]); // afficher l'erreur
			mini->exit_status = 1; // mettre le code de sortie a 1
			mini->cmd[i].out_fail = 1; // marquer que l'ouverture a echoue
			return ;
		}
		if (mini->cmd[i].fd_out != -1) // si un ancien fichier out existe,
			close(mini->cmd[i].fd_out); // on le ferme avant de le remplacer
		mini->cmd[i].fd_out = fd_temp; // puis on en ajoute le nouveau
		// ce fichier devient la sortie active (le dernier redir qui va s'effectuer)
		n++;
	}
}

// appliquer la redirection infile (<) pour la commande i
int	appliquer_infile(t_mini *mini, int i)
{
	int	n;
	int	fd_temp;

	n = 0;
	fd_temp = -1;
	if (!mini || i < 0 || i >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return (-1);
	if (!mini->cmd) // si cmd n'existe pas
		return (-1);
	if (mini->cmd[i].in_fail || mini->cmd[i].out_fail) // si deja echec de redir in ou out, on ne fait rien
		return (0);
	if (mini->cmd[i].infile == NULL) // proteger au cas ou infile est NULL
	{
		mini->exit_status = 2;
		return (-1);
	}
	while (mini->cmd[i].infile[n]) // pour chaque fichier de redirection infile
	{
		fd_temp = open(mini->cmd[i].infile[n], O_RDONLY);
		// a chaque iteration de infile[n], ouvrir le fichier en lecture seule dans un fd temporaire
		if (fd_temp < 0) // si echec d'ouverture
		{
			perror(mini->cmd[i].infile[n]); // afficher l'erreur
			mini->exit_status = 1; // mettre le code de sortie a 1
			mini->cmd[i].fd_in = -1; // marquer que l'ouverture a echoue
			mini->cmd[i].in_fail = 1; // marquer que l'ouverture a echoue
			return (-1);
		}
		if (mini->cmd[i].fd_in != -1) // si un ancien fichier in existe,
			close(mini->cmd[i].fd_in); // on le ferme avant de le remplacer
		mini->cmd[i].fd_in = fd_temp; // reinitialiser fd_in
		// ce fichier devient l'entree active (le dernier redir qui va s'effectuer)
		n++;
	}
	return (0);
}

// Préparation du nom de fichier temporaire pour heredoc
// j: index de cmd
// n: index du heredoc pour cette cmd 
// ex) cmd1 << limiter1 << limiter2 -> n = 0 pour limiter1, n = 1 pour limiter2
int	preparer_temp_file_name(t_mini *mini, int j, int n)
{
	char	*index_char;
	char	*temp_j;
	char	*tiret_bas;
	char	*temp_j_n;

	if (!mini || j < 0 || j >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return (-1);
	if (n < 0 || n >= mini->cmd[j].compter_heredoc) // si n est invalide (ex. n = 2 alors que compter_heredoc = 2, donc les index valides sont 0 et 1)
		return (-1);
	if (mini->cmd[j].temp_heredoc && mini->cmd[j].temp_heredoc[n]) // si un ancien fichier temporaire existe deja pour ce heredoc[n], on le supprime
	{
		unlink(mini->cmd[j].temp_heredoc[n]); // supprimer l'ancien fichier temporaire s'il existe
		free(mini->cmd[j].temp_heredoc[n]); // free l'ancien nom de fichier temporaire
		mini->cmd[j].temp_heredoc[n] = NULL; // reinitialiser a NULL
	}
	index_char = ft_itoa(j); // convertir l'index j en string pour nommer l'index du fichier temporaire (temp_j)
	if (!index_char)
		return (perror("index j (itoa)"), -1);
	temp_j = ft_strjoin("temp_", index_char); // construire le nom du fichier temporaire pour heredoc: temp_j
	free(index_char); // liberer index_char apres utilisation
	if (!temp_j) // si echec de malloc pour le nom du fichier temporaire
		return (perror("strjoin: temp_ + j"), -1);
	tiret_bas = ft_strjoin(temp_j, "_"); // ajouter un tiret bas entre temp_j et n pour le nom du fichier temporaire (temp_j_n)
	free(temp_j); // liberer temp_j apres utilisation
	if (!tiret_bas)
		return (perror("strjoin: temp_j + _"), -1);
	index_char = ft_itoa(n); // l'index n en char pour le fichier temporaire (temp_j_n)
	if (!index_char)
		return (perror("itoa index n"), free(tiret_bas), -1);
	temp_j_n = ft_strjoin(tiret_bas, index_char); // construire le nom du fichier temporaire pour heredoc: temp_j_n
	free(tiret_bas); // liberer temp_j apres utilisation
	free(index_char); // liberer index_char apres utilisation
	if (!temp_j_n) // si echec de malloc pour le nom du fichier temporaire
		return (perror("malloc: file name j n"), -1);
	mini->cmd[j].temp_heredoc[n] = temp_j_n; // assigner le nom du fichier temporaire a mini->cmd[j].temp_heredoc
	if (access(mini->cmd[j].temp_heredoc[n], F_OK) == 0) // si le fichier mini->cmd[j].temp_heredoc existe deja
		unlink(mini->cmd[j].temp_heredoc[n]); // supprimer le fichier existant
	return (0);
}

// afficher le message d'erreur quand on saisit ctrl d dans heredoc
void	print_heredoc_warning_ctrl_d(char *delimiter)
{
	if (!delimiter)
		delimiter = "";
	write(2, "warning: here-document delimited by end-of-file (wanted '", 57);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
	// le nombre de lines a faire apres ******************************************************
}

// recuperer les lignes de heredoc, puis les stocker dans le fichier temp
// fd = fd de fichier temporaire temp, delimiter = limiter
// int	collecter_heredoc_lines(int fd, char *delimiter)
int	collecter_heredoc_lines(int fd, t_mini *mini, int j, int n)
{
	char	*line;
	char	*line_applique;

	if (!mini || !mini->cmd
		|| j < 0 || j >= mini->nbr_cmd || !mini->cmd[j].limiter
		|| n < 0 || n >= mini->cmd[j].compter_heredoc || !mini->cmd[j].hd_env)
		return (-1);
	if (!mini->cmd[j].limiter[n])
		return (1);
	while (1)
	{
		line = readline("> "); // afficher un prompte qui ressemble a heredoc
		if (!line) // saisit ctrl+D -> on quitte
			return (1); // dans ce cas (ctrl+D), pas besoin de liberer le memoire, puisqu'il y en a pas
		if (mini->cmd[j].limiter[n] && ft_strcmp(line, mini->cmd[j].limiter[n]) == 0) // quand on croise limiter -> on quitte
		// si delimiter est NULL -> erreur (pour proteger on ajoute dans la condition delimiter aussi)
		{
			free(line); // liberer readline
			return (0); // quitte la boucle (et cette fonction)
		}

		line_applique = line;
		if (mini->cmd[j].hd_env[n])
		{
			line_applique = remplacer_dollar(line, mini);
			if (!line_applique)
				return (free(line), -1);
		}

		if (write(fd, line_applique, ft_strlen(line_applique)) == -1 || write(fd, "\n", 1) == -1) // ecrire la ligne dans le fichier temp (le resultat de line + '\n')
		// le resultat de readline n'applique pas automatiquement '\n', on en ajoute a la fin
		// si write retourne -1, c'est une erreur d'ecriture, on libere readline et on retourne -1
		{
			if (mini->cmd[j].hd_env[n])
				free(line_applique);
			free(line);
			return (-1); // si echec d'ecriture
		}
		if (mini->cmd[j].hd_env[n])
			free(line_applique);
		free(line); // free readline avant de quitter la fonction hihi
	}
	return (0);
}

// verifier s'il y a une quote paire dans limiter
// 1: on trouve au moins une quote bien fermee dans limiter
// 0: aucune quote paire ou pas de quote
int	check_quote_limiter(char *limiter)
{
	int		i;
	char	quote;

	i = 0;
	if (!limiter)
		return (0);
	while (limiter[i] != '\0')
	{
		if (limiter[i] == '\'' || limiter[i] == '"') // si on croise une quote
		{
			quote = limiter[i]; // sauvegarder la premiere quote
			i++; // avancer pour chercher la quote fermante
			while (limiter[i] && limiter[i] != quote) // tout ce qui n'est pas la meme quote, on passe
				i++;
			if (limiter[i] == quote) // 1 si on trouve la meme quote paire
				return (1);
			else
				return (0); // 0 quote ouverte mais pas fermee
		}
		i++;
	}
	return (0); // pas de quote
}

// verifier soit on applique l'expansion de l'env ou non par rapport a limiter
int	check_heredoc_env(char *limiter)
{
	if (!check_quote_limiter(limiter))
		return (1); // 1 s'il y a pas de quote dans limiter (on applique l'expansion de l'env)
	return (0); // 0 sinon (on n'applique pas l'expansion de l'env, on les lit comme un caractere)
}


// appliquer heredoc dans le processus enfant
// le processus enfant: ecrire les lignes du heredoc n dans le fichier temporaire temp_heredoc[n] jusqu'a ce qu'on arrive a limiter[n], puis quitter
void	appliquer_heredoc_enfant(t_mini *mini, int j, int n)
{
	int	resultat;
	int	fd_temp;

	signal(SIGINT, SIG_DFL); // quand on saisit ctrl-C, le processus enfant doit etre termine (ctrl-C doit finir seulement le processus enfant)
	signal(SIGQUIT, SIG_IGN); // quand on saisit ctrt-\, on l'ignore (ca change rien)
	if (!mini || j < 0 || j >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		exit (1);
	if (n < 0 || n >= mini->cmd[j].compter_heredoc) // si n est invalide (ex. n = 2 alors que compter_heredoc = 2, donc les index valides sont 0 et 1)
		exit (1);
	if (!mini->cmd[j].temp_heredoc || !mini->cmd[j].temp_heredoc[n]) // si le nom du fichier n'existe pas, on finit
		exit (1);
	fd_temp = open(mini->cmd[j].temp_heredoc[n], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// ouvrir (créer) le fichier temporaire en écriture (on ecrase le contenu prcedent)
	if (fd_temp == -1)
	{
		perror("open temp");
		exit (1);
	}
	if (!mini->cmd[j].limiter || !mini->cmd[j].limiter[n]) // si limiter est NULL, c'est une erreur, on finit
	{
		close(fd_temp);
		exit (1);
	}
	resultat = collecter_heredoc_lines(fd_temp, mini, j, n);
	// collecter des lignes heredoc dans le fichier temp
	// (lire des lignes jusqu'a ce qu'on arrive limiter[n], puis les ecrire dans le fichier temp)
	if (resultat == 1) // ctrl-D
	{
		print_heredoc_warning_ctrl_d(mini->cmd[j].limiter[n]);
		close(fd_temp);
		exit (0);
	}
	if (resultat < 0) // erreur dans collecter_heredoc_lines
	{
		perror("collecter heredoc");
		close(fd_temp);
		exit (1);
	}
	close(fd_temp); //fermer le fd temp dans l'enfant pour eviter les leak
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

// appliquer heredoc pour la commande j
int	appliquer_heredoc_cmd(t_mini *mini, int j)
{
	int	status; // pour waitpid (si le processus enfant s'est termine correctement)
	int	exit_status; // pour resultat du waitpid (code de sortie du processus enfant)
	int	exit_signal;
	int	n;

	n = 0;
	if (!mini || j < 0 || j >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return (-1);
	if (mini->cmd[j].compter_heredoc <= 0) // si il n'y a pas de heredoc pour cette cmd, on ne fait rien
		return (0);

	signal(SIGINT, SIG_IGN); // le processus parent ignore ctrl+C pendant le processus enfant (heredoc fork)
	signal(SIGQUIT, SIG_IGN); // le processus parent ignore ctrl+\ pendant le processus enfant (heredoc fork)

	if (mini->cmd[j].temp_heredoc == NULL) // il faut d'abord allouer le tableau temp_heredoc[] pour stocker les noms des fichiers temporaires de chaque heredoc, avant de remplir temp_heredoc[n]
	{
		mini->cmd[j].temp_heredoc = ft_calloc(mini->cmd[j].compter_heredoc + 1, sizeof(char *)); // +1 pour NULL (terminaison)
		if (!mini->cmd[j].temp_heredoc)
		{
			mini->cmd[j].in_fail = 1;
			mini->exit_status = 1;
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
	}
	while (n < mini->cmd[j].compter_heredoc) // pour chaque heredoc de cette cmd, on prepare le nom du fichier temporaire
	{
		if (preparer_temp_file_name(mini, j, n) == -1)
		{
			mini->cmd[j].in_fail = 1;
			mini->exit_status = 1;
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
		mini->cmd[j].pid_heredoc = fork(); // creer un processus enfant pour gerer heredoc
		if (mini->cmd[j].pid_heredoc == -1) // si echec de fork
		{
			mini->cmd[j].in_fail = 1;
			mini->exit_status = 1;
			init_signaux();
			return (-1); // si echec de waitpid
		}
		if (mini->cmd[j].pid_heredoc == 0) // processus enfant
			appliquer_heredoc_enfant(mini, j, n);
		if (waitpid(mini->cmd[j].pid_heredoc, &status, 0) == -1) // attendre la fin du processus enfant
		{
			init_signaux();
			mini->cmd[j].in_fail = 1;
			mini->exit_status = 1;
			return (-1); // si echec de waitpid
		}
		// init_signaux(); // apres la fin du processus enfant, on applique des signaux pareils que shell
		if (WIFSIGNALED(status))
		{
			exit_signal = WTERMSIG(status);
			mini->cmd[j].in_fail = 1;
			mini->exit_status = 128 + exit_signal;
			init_signaux();
			return (-1);
		}
		else if (WIFEXITED(status)) // si le processus enfant s'est termine correctement
		{
			exit_status = WEXITSTATUS(status); // recuperer le code de sortie
			if (exit_status != 0) // si le code de sortie n'est pas 0 (erreur dans heredoc)
			{
				mini->cmd[j].in_fail = 1; // marquer l'echec de heredoc
				mini->exit_status = exit_status; // mettre a jour le code de sortie global
				init_signaux(); // reinitialiser les signaux avant de retourner
				return (-1); // retourner -1 pour indiquer l'erreur
			}
		}
		else // si le processus enfant ne s'est pas termine correctement
		{
			mini->cmd[j].in_fail = 1; // marquer l'echec de heredoc
			mini->exit_status = 1; // mettre a jour le code de sortie global
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
		if (!mini->cmd[j].temp_heredoc || !mini->cmd[j].temp_heredoc[n]) // proteger au cas ou temp_heredoc est NULL
		{
			mini->cmd[j].in_fail = 1;
			mini->exit_status = 1;
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
		n++;
	}
	if (mini->cmd[j].fd_in != -1) // si fd_in est deja ouvert, on le ferme d'abord
		close(mini->cmd[j].fd_in); // fermer l'ancien fd_in avant de le remplacer
	mini->cmd[j].fd_in = open(mini->cmd[j].temp_heredoc[mini->cmd[j].compter_heredoc - 1], O_RDONLY); // reouvrir le fichier temp en lecture seule
	if (mini->cmd[j].fd_in == -1) // si echec d'ouverture de temp en lecture
	{
		mini->cmd[j].in_fail = 1;
		mini->exit_status = 1;
		perror("open temp for reading"); // afficher l'erreur
		init_signaux(); // reinitialiser les signaux avant de retourner
		return (-1);
	}
	init_signaux(); // reinitialiser les signaux avant de retourner
	return (0);
}




// ========================================= test ===========================================
// ==========================================================================================

// juste pour tester les redir 
static void	print_preview_path(const char *path)
{
	int		fd;
	char	buf[256];
	ssize_t	n;

	if (!path)
		return ;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("preview open");
		return ;
	}
	n = read(fd, buf, sizeof(buf) - 1);
	if (n > 0)
	{
		buf[n] = '\0';
		printf("    [preview] ----\n%s\n    ------------\n", buf);
	}
	else
		printf("    [preview] (empty)\n");
	close(fd);
}


static void	print_in_redir_order(t_cmd *cmd)
{
	int k;

	if (!cmd)
		return ;

	printf("order: compter_in_hd=%d\n", cmd->compter_in_hd);

	printf("order: in_heredoc=");
	if (!cmd->in_heredoc)
		printf("(null)\n");
	else
	{
		k = 0;
		while (k < cmd->compter_in_hd)
		{
			printf("%d ", cmd->in_heredoc[k]); // 0=< , 1=<<
			k++;
		}
		printf("\n");
	}

	printf("order: in_hd_index=");
	if (!cmd->in_hd_index)
		printf("(null)\n");
	else
	{
		k = 0;
		while (k < cmd->compter_in_hd)
		{
			printf("%d ", cmd->in_hd_index[k]); // index dans infile[] ou limiter[]
			k++;
		}
		printf("\n");
	}
}



// ca aussi juste pour tester redir j'en ai maaaaaare
void	test_redirs(t_mini *mini)
{
	int	i;

	if (!mini || !mini->cmd)
		return ;

	i = 0;
	while (i < mini->nbr_cmd)
	{
		printf("\n========== CMD %d ==========\n", i);

		// 0) dump parsing infos (infile/limiter/temp/outfile)
		printf("raw: infile=");
		if (mini->cmd[i].infile)
		{
			int k = 0;
			while (mini->cmd[i].infile[k])
			{
				printf("[%s] ", mini->cmd[i].infile[k]);
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		printf("raw: limiter=");
		if (mini->cmd[i].limiter)
		{
			int k = 0;
			while (mini->cmd[i].limiter[k])
			{
				printf("[<< %s] ", mini->cmd[i].limiter[k]);
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		printf("raw: temp_heredoc=");
		if (mini->cmd[i].temp_heredoc)
		{
			int k = 0;
			while (mini->cmd[i].temp_heredoc[k])
			{
				printf("[%s] ", mini->cmd[i].temp_heredoc[k]);
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		printf("raw: outfile=");
		if (mini->cmd[i].outfile)
		{
			int k = 0;
			while (mini->cmd[i].outfile[k])
			{
				int mode = 0;
				if (mini->cmd[i].out_append)
					mode = mini->cmd[i].out_append[k];
				printf("[%s mode=%s] ", mini->cmd[i].outfile[k], mode ? ">>" : ">");
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		// NEW: print in redir order arrays
		print_in_redir_order(&mini->cmd[i]);

		// 1) appliquer "in" redirections selon l'ordre (0=< , 1=<<)
		//    (pour tester: on simule juste l'effet final -> dernier redir gagne)
		if (mini->cmd[i].compter_in_hd > 0 && !mini->cmd[i].in_fail)
		{
			int k = 0;
			while (k < mini->cmd[i].compter_in_hd)
			{
				int kind = mini->cmd[i].in_heredoc ? mini->cmd[i].in_heredoc[k] : -1;
				int idx  = mini->cmd[i].in_hd_index ? mini->cmd[i].in_hd_index[k] : -1;

				if (kind == 0) // '<' -> infile[idx]
				{
					if (!mini->cmd[i].infile || idx < 0 || !mini->cmd[i].infile[idx])
						printf("    [in order] k=%d: <  idx=%d  (INVALID)\n", k, idx);
					else
						printf("    [in order] k=%d: <  idx=%d  file=%s\n", k, idx, mini->cmd[i].infile[idx]);
				}
				else if (kind == 1) // '<<' -> limiter[idx]
				{
					if (!mini->cmd[i].limiter || idx < 0 || !mini->cmd[i].limiter[idx])
						printf("    [in order] k=%d: << idx=%d  (INVALID)\n", k, idx);
					else
						printf("    [in order] k=%d: << idx=%d  limiter=%s\n", k, idx, mini->cmd[i].limiter[idx]);
				}
				else
					printf("    [in order] k=%d: kind=%d idx=%d (UNKNOWN)\n", k, kind, idx);

				k++;
			}
		}

		// 2) heredoc (si parsing a detecte au moins un <<)
		if (mini->cmd[i].compter_heredoc > 0)
		{
			if (appliquer_heredoc_cmd(mini, i) == -1)
				printf("    heredoc: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    heredoc: OK (fd_in=%d)\n", mini->cmd[i].fd_in);

			// preview last heredoc temp file (celui qui sera utilisé si dernier in redir est <<)
			if (!mini->cmd[i].in_fail && mini->cmd[i].temp_heredoc)
			{
				int last = 0;
				while (mini->cmd[i].temp_heredoc[last])
					last++;
				if (last > 0)
					print_preview_path(mini->cmd[i].temp_heredoc[last - 1]);
			}
		}

		// 3) infile (pour tester: on l'applique aussi, mais en vrai ça dépend de l'ordre)
		if (mini->cmd[i].infile && mini->cmd[i].infile[0])
		{
			if (appliquer_infile(mini, i) == -1)
				printf("    infile: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    infile: OK (fd_in=%d)\n", mini->cmd[i].fd_in);

			// preview last infile
			if (!mini->cmd[i].in_fail)
			{
				int last = 0;
				while (mini->cmd[i].infile[last])
					last++;
				if (last > 0)
					print_preview_path(mini->cmd[i].infile[last - 1]);
			}
		}

		// 4) outfile
		if (mini->cmd[i].outfile && mini->cmd[i].outfile[0])
		{
			process_out_redir(mini, i);
			if (mini->cmd[i].out_fail)
				printf("    outfile: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    outfile: OK (fd_out=%d)\n", mini->cmd[i].fd_out);
		}

		// resume
		printf("[summary] fd_in=%d fd_out=%d in_fail=%d out_fail=%d exit_status=%d\n",
			mini->cmd[i].fd_in, mini->cmd[i].fd_out,
			mini->cmd[i].in_fail, mini->cmd[i].out_fail, mini->exit_status);

		i++;
	}
	printf("\n============================\n");
}



// liste free
void	free_tokens(t_token **token)
{
	t_token	*tmp;

	while (*token)
	{
		tmp = *token;
		*token = (*token)->next;
		free(tmp->str); // free la string du token
		free(tmp); // free le token lui-meme
	}
}

// free le tableau de string (ex. infile[], outfile[], temp_heredoc[], limiter[])
void	free_tab_char(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

// free le tableau d'int (ex. out_append[], in_heredoc[], in_hd_index[], hd_env[])
void	free_tab_int(int *tab)
{
	if (!tab)
		return ;
	free(tab);
}

// supprimer tous les fichiers temporaires de heredoc (unlink), puis free le tableau temp_heredoc[]
void	free_temp_heredoc(char **temp)
{
	int	i;

	if (!temp)
		return ;
	i = 0;
	while (temp[i])
	{
		unlink(temp[i]); // supprimer le fichier temporaire du heredoc
		free(temp[i]); // free le nom du fichier temporaire du heredoc
		i++;
	}
	free(temp); // free le tableau de string (char **) apres avoir supprimer les fichiers temporaires
}

// free les fd de cmd, puis free les tableaux infile[], outfile[], temp_heredoc[], limiter[], out_append[], in_heredoc[], in_hd_index[] de cmd
void	free_cmd_fd_tab(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->fd_in != -1)
		close(cmd->fd_in);
	cmd->fd_in = -1;
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
	cmd->fd_out = -1;
	free_tab_char(cmd->cmd);
	cmd->cmd = NULL;
	free_tab_char(cmd->infile);
	cmd->infile = NULL;
	free_tab_char(cmd->outfile);
	cmd->outfile = NULL;
	free_tab_char(cmd->temp_heredoc);
	cmd->temp_heredoc = NULL;
	free_tab_char(cmd->limiter);
	cmd->limiter = NULL;
	free_tab_int(cmd->out_append);
	cmd->out_append = NULL;
	free_tab_int(cmd->in_heredoc);
	cmd->in_heredoc = NULL;
	free_tab_int(cmd->in_hd_index);
	cmd->in_hd_index = NULL;
}

// free tous les cmd (fd, tableaux de string et d'int), puis free le tableau de cmd
void	free_cmd_all(t_cmd *cmd, int nbr_cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (i < nbr_cmd) // pour chaque cmd, free les fd et les tableaux de string et d'int
	{
		free_cmd_fd_tab(&cmd[i]); // free les fd et les tableaux de string et d'int de cmd[i]
		i++;
	}
	free(cmd); // free le tableau de cmd apres avoir free tous les cmd
}

// free la structure mini apres avoir free tous les cmd
// env: on utilise directement le env de main, donc on ne le free pas (c'est le env de main, pas une copie)
void	free_mini(t_mini *mini)
{
	if (!mini)
		return ;
	free_cmd_all(mini->cmd, mini->nbr_cmd); // free tous les cmd (fd, tableaux de string et d'int), puis free le tableau de cmd
	free(mini); // free la structure mini apres avoir free tous les cmd
}


int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
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
		mini->exit_status = 0;
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
		parse_input(line, &parsing, mini);
		
		temp = parsing;
		while (temp)
		{
			printf("testing tokens:\n");
			printf("noeud %d '%s' | type %s | type_quote %s\n", i, temp->str, get_token_type_str(temp->type_token), get_token_type_state(temp->type_quote));
			i++;
			temp = temp->next;
		}
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

		test_print_cmds(cmd, count_pipe(parsing) + 1);
		test_redirs(mini);
		if (parse_input(line, &parsing, mini) < 0)
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

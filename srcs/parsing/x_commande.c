#include "minishell.h"

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

// fonction pour agrandir un tableau et rajouter une chaine, comme pour le builtin export
// char **tab : double tableau actuel, *str : nouveau tableau a ajouter,
// int  size : taille actuelle de double tableau
char** add_double_tab_char(char **tab, char *str, int size)
{
	char **new_tab; // nouveau double tableau agrandi
	int j; // index pour parcourir les tableaux

	if (!tab && size > 0) // proteger au cas ou tab est NULL mais size > 0
		return (NULL);
	if (!str || size < 0) // proteger au cas ou str est NULL ou size < 0
		return (NULL);
	new_tab = malloc(sizeof(char *) * (size + 2)); // +1 pour le nouveau +1 pour NULL hihi
	if (!new_tab)
			return (NULL);
	j = 0;
	if (tab) // si tab n'est pas NULL, on copie les valeurs existantes
	{
		while (j < size)
		{
			new_tab[j] = tab[j]; // copier l'adresse de chaque chaine
			// cf) on copie juste le pointeur (l'adresse)
			// chaque tab[j] est un pointeur vers une chaine de caracteres
			// cf) char **tab = *tab[] = {"pho", "malatang", NULL}; (tableau de pinteurs vers des chaines)
			//     char tab[j] -> tab[0] = adresse de "pho", tab[1] = adresse de "malatang" (chaine de caracteres)
			j++;
		}
	}
	new_tab[j] = str; // l'adresse de nouvelle chaine ajoutee, on ajoute la nouvelle adresse ici youpiii
	new_tab[j + 1] = NULL; // terminer par NULL
	free(tab); // vu qu'on a bien cree un nouveau tableau agrandi, on libere l'ancien tableau de pointeurs
	return (new_tab);
}

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
	if (tab) // si tab n'est pas NULL, on copie les valeurs existantes
	{
		while (j < size)
		{
				new_tab[j] = tab[j]; // copier chaque valeur
				j++;
		}
	}
	new_tab[j] = val; // ajouter la nouvelle valeur (1 ou 0)
	free(tab); // vu qu'on a bien cree un nouveau tableau agrandi, on libere l'ancien tableau
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
	char	**new_tab_char; // temporaire pour le nom de la commande (pour proteger)
	// si l'un des fonctions add_tab_char ou add_tab_int retourne NULL, ca risque de perdre tous les pointeurs qui etaient deja dans le tableau cmd, donc on utilise des pointeurs temporaires pour proteger (pour garder des anciens pointeurs)
	int		*new_tab_int; // temporaire pour le tableau int (pour proteger)

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
					return (free(cmd[index_cmd].cmd), cmd[index_cmd].cmd = NULL, -1);
				cmd[index_cmd].cmd[1] = NULL; // vu que c'est un double tableau (tableau de pointeurs char *, donc argv), on place d'abord le NULL final
			}
			else // s'il y a deja un argument dans le double tableau, on agrandit le tableau (pour ajouter un nouvel arguement)
			{
				mot_temp = ft_strdup(token->str); // dupliquer le contenu de token->str (pour proteger)
				if (!mot_temp)
					return (-1);
				new_tab_char = add_double_tab_char(cmd[index_cmd].cmd, mot_temp, i); // agrandir le tableau cmd[index_cmd].cmd pour ajouter le nouveau mot
				// ex) tab[0] = {"echo", NULL} -> {"echo", "hihi", NULL}
				// on le sauvegarde d'abord dans un pointeur temporaire pour proteger au cas ou add_double_tab_char retourne NULL (perte de tous les pointeurs dans cmd[index_cmd].cmd)
				if (!new_tab_char)
					return (free(mot_temp), -1);
				cmd[index_cmd].cmd = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
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

			new_tab_char = add_double_tab_char(cmd[index_cmd].infile, file_temp, size_file_tab);
			// agrandir le tableau infile pour ajouter le nouveau fichier
			// ex) cat < file1 < file2  -> infile = {"file1", "file2", NULL}
			// on n'ecrase plus, on stocke tout dans l'ordre
			if (!new_tab_char)
				return (free(file_temp), -1);
			cmd[index_cmd].infile = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].infile)
				return (-1);
			new_tab_int = add_double_tab_int(cmd[index_cmd].in_heredoc, 0, cmd[index_cmd].compter_in_hd);
			// ajouter le type de in redir: dans ce cas c'est in (<), on ajoute 0
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].in_heredoc = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].in_heredoc)
				return (-1);
			new_tab_int = add_double_tab_int(cmd[index_cmd].in_hd_index, size_file_tab, cmd[index_cmd].compter_in_hd);
			// sauvegarde l'index de infile de cette in redir
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].in_hd_index = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
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
			new_tab_char = add_double_tab_char(cmd[index_cmd].outfile, file_temp, size_file_tab); // agrandir le tableau outfile pour ajouter le nouveau fichier
			if (!new_tab_char)
				return (free(file_temp), -1);
			cmd[index_cmd].outfile = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].outfile)
				return (-1);
			new_tab_int = add_double_tab_int(cmd[index_cmd].out_append, 0, size_file_tab); // agrandir le tableau out_append pour ajouter 0 (truncate) <- puisqu'on est dans la condition T_FD_OUT >
			// 0='>' (truncate), 1='>>' (append)
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].out_append = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].out_append)
				return (-1);
		}
		else if (token->type_token == T_FD_OUT_APPEND)
		{
			file_temp = ft_strdup(token->str); // dupliquer le nom du fichier (redirection >>) pour stocker dans cmd
			if (!file_temp)
				return (-1);
			size_file_tab = len_tab_char(cmd[index_cmd].outfile); // compter la taille actuelle du tableau outfile
			new_tab_char = add_double_tab_char(cmd[index_cmd].outfile, file_temp, size_file_tab); // agrandir le tableau outfile pour ajouter le nouveau fichier
			if (!new_tab_char)
				return (free(file_temp), -1);
			cmd[index_cmd].outfile = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].outfile)
				return (-1);
			new_tab_int = add_double_tab_int(cmd[index_cmd].out_append, 1, size_file_tab); // agrandir le tableau out_append pour ajouter 1 (append) <- puisqu'on est dans la condition T_FD_OUT_APPEND >>
			// 0='>' (truncate), 1='>>' (append)
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].out_append = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
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

			new_tab_char = add_double_tab_char(cmd[index_cmd].limiter, limiter_sans_quote, n); 
			// sauvegarder et agrandir le tableau limiter (sans quote) pour ajouter le nouveau limiter
			if (!new_tab_char)
				return (free(limiter_sans_quote), -1);
			cmd[index_cmd].limiter = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].limiter)
				return (-1);
			new_tab_int = add_double_tab_int(cmd[index_cmd].hd_env, limiter_env, n);
			// sauvegarder le type de l'expansion de l'env de chaque cas
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].hd_env = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].hd_env)
				return (-1);

			cmd[index_cmd].compter_heredoc++; // incrementer le nombre de heredoc pour cette commande
			cmd[index_cmd].heredoc = 1; // marquer que c'est heredoc (<<)
			new_tab_int = add_double_tab_int(cmd[index_cmd].in_heredoc, 1, cmd[index_cmd].compter_in_hd);
			// ajouter le type de in redir: dans ce cas c'est heredoc (<<), on ajoute 1
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].in_heredoc = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].in_heredoc)
				return (-1);
			new_tab_int = add_double_tab_int(cmd[index_cmd].in_hd_index, n, cmd[index_cmd].compter_in_hd);
			// sauvegarde l'index de infile de cette heredoc redir
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].in_hd_index = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
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

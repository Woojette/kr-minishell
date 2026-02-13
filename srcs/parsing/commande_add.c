#include "minishell.h"

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
	int		size_cmd;

	index_cmd = 0;
	i = 0;
	n = 0;
	redir_existe = 0;
	mot_temp = NULL;
	file_temp = NULL;
	size_file_tab = 0;
	limiter_sans_quote = 0;
	limiter_env = 0;
	size_cmd = 0;
	
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
				size_cmd = len_tab_char(cmd[index_cmd].cmd);
				new_tab_char = add_double_tab_char(cmd[index_cmd].cmd, mot_temp, size_cmd); // agrandir le tableau cmd[index_cmd].cmd pour ajouter le nouveau mot
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
		else if (token->type_token == T_FD_IN || token->type_token == T_FD_OUT || token->type_token == T_FD_OUT_APPEND)
		{
			file_temp = ft_strdup(token->str); // dupliquer le nom du fichier (redirection >) pour stocker dans cmd
			if (!file_temp)
				return (-1);
			size_file_tab = len_tab_char(cmd[index_cmd].inoutfile); // compter la taille actuelle du tableau outfile
			new_tab_char = add_double_tab_char(cmd[index_cmd].inoutfile, file_temp, size_file_tab); // agrandir le tableau outfile pour ajouter le nouveau fichier
			if (!new_tab_char)
				return (free(file_temp), -1);
			cmd[index_cmd].inoutfile = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].inoutfile)
				return (-1);
			if (token->type_token == T_FD_IN)
				new_tab_int = add_double_tab_int(cmd[index_cmd].ihoa, 3, size_file_tab);
			if (token->type_token == T_FD_OUT)
				new_tab_int = add_double_tab_int(cmd[index_cmd].ihoa, 1, size_file_tab); // agrandir le tableau out_append pour ajouter 0 (truncate) <- puisqu'on est dans la condition T_FD_OUT >
			// 0='>' (truncate), 1='>>' (append)
			if (token->type_token == T_FD_OUT_APPEND)
				new_tab_int = add_double_tab_int(cmd[index_cmd].ihoa, 2, size_file_tab);
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].ihoa = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].ihoa)
				return (-1);
			cmd[index_cmd].compter_ihoa++; // 얘 원래 없던데 왜지?
		}
		else if (token->type_token == T_FD_HEREDOC)
		{
			file_temp = ft_strdup("/dev/null"); // dupliquer le nom du fichier (redirection >) pour stocker dans cmd
			if (!file_temp)
				return (-1);
			size_file_tab = len_tab_char(cmd[index_cmd].inoutfile); // compter la taille actuelle du tableau outfile
			new_tab_char = add_double_tab_char(cmd[index_cmd].inoutfile, file_temp, size_file_tab);
			if (!new_tab_char)
				return (free(file_temp), -1);
			cmd[index_cmd].inoutfile = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			new_tab_int = add_double_tab_int(cmd[index_cmd].ihoa, 4, size_file_tab);
			// ajouter le type de in redir: dans ce cas c'est heredoc (<<), on ajoute 1
			if (!new_tab_int)
				return (-1);
			cmd[index_cmd].ihoa = new_tab_int; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
			if (!cmd[index_cmd].ihoa)
				return (-1);
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
			cmd[index_cmd].limiter = new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi //???
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
			cmd[index_cmd].compter_ihoa++; // des qu'on ajoute in redir, on incremente de 1 (l'index de in redir)
		}
		if (token->type_token == T_PIPE) // si on arrive a '|'
		{
			if (cmd[index_cmd].inoutfile || cmd[index_cmd].compter_ihoa > 0)
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
	if (index_cmd > 0 && cmd[index_cmd].cmd == NULL
			&& cmd[index_cmd].inoutfile == NULL && !cmd[index_cmd].heredoc)
			// proteger au cas ou il y a un pipe a la fin (ex: cmd1 | )
			// et aussi proteger le cas ou il y a un pipe avec redir mais sans mot (ex: cmd1 | < file)
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	if (cmd[index_cmd].cmd != NULL) // on termine aussi le dernier argv (apres la boucle)
		cmd[index_cmd].cmd[i] = NULL; // on ferme bien la fin 
	return (0); // pour distinguer du cas qui marche bien le cas d'erreur (-1)
}

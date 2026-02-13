#include "minishell.h"

void	init_cmd(t_cmd *cmd, int j)
{
	cmd[j].cmd = NULL; // on initialise tous les pointeurs a NULL (pour proteger)
	// redirections: tableaux (on garde tous les noms de fichiers dans l'ordre)
	// inoutfile 로 리다이렉션 이름 싹다 집어넣을거임 
	cmd[j].inoutfile = NULL; // liste des fichiers pour  < (char **, termine par NULL)
	cmd[j].temp_heredoc = NULL; // tableau (char **) des noms de fichiers temporaire pour heredoc (qui termine par NULL)
	cmd[j].limiter = NULL; // tableau (char **) des limiters pour heredoc (qui termine par NULL)
	cmd[j].compter_heredoc = 0; // pour compter le nombre de heredoc (<<) dans chaque commande
	cmd[j].in_heredoc = NULL; // tableau int pour sauvegarder le type de in redir (<, <<) par ordre
	cmd[j].compter_ihoa = 0; // ihoa 다 세어서 넣을것
	cmd[j].hd_env = NULL; // tableau int: appliquer env expansion pour chaque limiter[n] (1 env, 0 sinon)
	// ihoa 에다 인트 배열로 인아웃파일이랑 똑같은 인덱스에 리다이렉션 타입 넣을거임
	cmd[j].ihoa = NULL; // initialiser a NULL pour proteger // 여기다가 리다이렉션 타입 싹 집어넣을것
	// NULL au debut: on alloue et agrandit au fur et a mesure dans add_cmd()
	// cf) out_append[n] = 1 pour >> (append),  0 pour > (truncate) / 1 4 2 3 순으로 인 히어독 아웃 아펜드
	cmd[j].heredoc = 0; // <<
	cmd[j].pid_heredoc = -1; // pid pour heredoc (fork)
	cmd[j].fd_in = -1;
	cmd[j].fd_out = -1;
	cmd[j].inout_fail = 0;
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
		init_cmd(cmd, j);
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

	if ((!tab && size > 0) || !str || size < 0) // proteger au cas ou tab est NULL mais size > 0
		return (NULL);
	// if (!str || size < 0) // proteger au cas ou str est NULL ou size < 0
	// 	return (NULL);
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
	return (free(tab), new_tab); // vu qu'on a bien cree un nouveau tableau agrandi, on libere l'ancien tableau de pointeurs
}

// fonction pour agrandir un tableau et rajouter une valeur int (1 ou 0), comme pour out_append
int	*add_double_tab_int(int *tab, int val, int size)
{
	int *new_tab; // nouveau tableau agrandi
	int j; // index pour parcourir les tableaux

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

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
// # include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
// # include <term.h>
// # include <termcap.h>
# include <termios.h>
# include <unistd.h>

typedef enum s_state
{
	DQUOTES,
	SQUOTES,
	GENERAL,
}	t_state;

typedef enum s_type_token
{
	T_MOT,
	T_PIPE,
	T_RD_IN,
	T_RD_OUT,
	T_RD_APPEND,
	T_RD_HEREDOC,
	T_FD_IN,
	T_FD_HEREDOC,
	T_FD_OUT,
	T_FD_OUT_APPEND,
}	t_type_token;

typedef enum s_type_bi
{
	T_ECHO,
	T_CD,
	T_PWD,
	T_EXPORT,
	T_UNSET,
	T_ENV,
	T_EXIT,
}	t_type_bi;

typedef struct	s_quote // pour enlever_quote_dans_token
{
	int	i;
	int	n;
	int	s_quote; // 1 si on est dans une single quote, 0 sinon
	int	d_quote; // 1 si on est dans une double quote, 0 sinon
} t_quote;

typedef struct s_var_cmd
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
} t_var_cmd;

// structure token issue du parsing de la ligne de commande 
// (mot, pipe, redir, etc)
typedef struct s_token
{
	char			*str; // token = <<
	t_type_token	type_token; //= T_RD_HEREDOC
	t_type_bi		type_bi; // type builtin
	t_state			type_quote; // = GENERAL
	struct s_token	*next;
}	t_token;

// structure commande apres decoupage par pipe
// cmd = tableau de chaine de caracteres (char **)
// ex) cmd[0] = {"echo", "hihi", NULL}
typedef struct s_cmd
{
	char	**cmd; // tableau de chaine de caracteres pour la commande et ses arguments (sans redir) ex) cmd[0] = {"echo", "hihi", NULL}

	// cmd redir
	char	**inoutfile; // tableau de chaine de caracteres pour les fichiers de redirection de in (<), out (>), append (>>) et heredoc (<<) dans l'ordre d'apparition ex) inoutfile[0] = "file1" (pour < file1), inoutfile[1] = "file2" (pour > file2)

	char	**temp_heredoc; // tableau des fichiers temporaires pour heredoc
	char	**limiter; // tableau des limiters (sauvegarder pls limiters)
	int		compter_heredoc; // le nombre de heredoc

	int		*in_heredoc; // tableau qui sauvegarde le type de in redir (<, <<), 1 si redirection heredoc (<<), 0 sinon (<)
	// int		*in_hd_index; // tableau des index de chaque infile(<) et de limiter(<<) qui concerne chaque redir de in_heredoc
	int		*ihoa; // tableau qui sauvegarde le type de redirection (in, heredoc, out, append) pour chaque redir de inoutfile, 1 pour > (truncate), 2 pour >> (append), 3 pour < (infile), 4 pour << (heredoc)
	int		compter_ihoa; // compter le nombre de redirections de in et heredoc pour chaque commande, pour savoir la taille du tableau in_heredoc et ihoa

	int		heredoc; // 1 si redirection heredoc (<<), 0 sinon

	int		*hd_env; // 1 applique env pour heredoc, 0 sinon

	pid_t	pid_heredoc; // pid du processus heredoc
	// resultat des fd
	int		fd_in; // resultat ouverture fichier de < or <<
	int		fd_out; // resultat ouverture fichier de > or >>
	// erreurs de redirection
	// int		in_fail; // 1 si echec ouverture fichier de < or <<, 0 sinon
	// int		out_fail; // 1 si echec ouverture fichier de > or >>, 0 sinon
	int	inout_fail; // 1 si echec ouverture fichier de <, <<, > ou >>, 0 sinon
}	t_cmd;


// structure du contexte global minishell 
// ex. env et exit status (dernier code de sortie)
typedef struct s_mini
{
	char	**env;
	int		exit_status;
	t_cmd	*cmd_array; // tableau de structures cmd (divise par pipe) // t_cmd *cmd_array로 바꿔서 씀
	int		nbr_cmd; // nombre de commandes (nombre de structures cmd dans cmd_tab)
	int	    pipe_read_end; // fd de lecture du pipe pour la commande precedente, pour connecter avec la commande suivante
	char	**path_array; // tableau des chemins d'acces pour les commandes (recupere a partir de la variable d'env PATH divise par ':')
}	t_mini;

// typedef struct s_mini
// {
// 	char	**env;
// 	int		exit_status;
// 	t_cmd	*cmd; // tableau de structures cmd (divise par pipe)
// 	int		nbr_cmd; // nombre de commandes (nombre de structures cmd dans cmd_tab)
// }	t_mini;



// ======================================================= token =======================================================

// token_all.c
int 	add_token(char *line, t_type_token type_token, int len, t_token **token); // ajouter des token dans la structure
int		new_node_init(t_token **new_node, char *line, int len, t_type_token type_token);
int		parse_input(char *line, t_token **token, t_mini *mini); // mettre des token a chaque noeud (mot, redir, pipe) 
void 	parse_fd_tokens(t_token **token); // pour la condition de token MOT (redir, fd)
const char	*get_token_type_str(t_type_token type);

// token_quote.c
// quote est le premier caractere
int		check_quote_debut_ok(char *line); // verifier s'il y a 2 quotes pareils dans la chaine de caracteres
int		check_2_quotes_debut_puis_fin(char *line); // fonction qui verifie (' ', '\0', redir, pipe) apres la 2e quote
// quote au milieu
int		check_quotes(char *line); // Verifier une quote qui n'est pas fermee
int		check_quote_milieu_ok(char *line);
int 	check_avant_quote_espace(char *line); // verifier s'il y a espace avant la quote au milieu
char	caractere_quote_debut(char *line); // recuprer le caractere de la premiere quote
int		index_quote_debut(char *line, char c); // recuperer l'index de la premiere quote 
int		index_quote_fin(char *line, char c); // recuperer l'index de la deuxieme quote 
int 	check_2_quotes_milieu_puis_fin(char *line); // fonction qui verifie (' ','\0', redir, pipe) apres la 2e quote

// token_len_mot.c
// pour compter len type mot
int		len_mot_total(char *line); // compter len du type mot (avec quote + sans quote)
int		len_mot_avant_quote(char *line); // recuperer len avant la quote qui est au milieu de la chaine
int		len_mot_2_quotes_entier(char *line); // compter le nombre de caracteres entre 2 quotes, y compris les 2 quotes
int		len_mot_apres_quote(char *line); // recuperer len apres la 2e quote
int		len_tab_char(char **tab); // compter le nombre de chaines dans un double tableau
// pas de quote dans la chaine
int		len_mot_sans_quote(char *line); // compter le nombre de caracteres s'il y a pas de 2 quotes qui fonctionnent

// pipe_check.c
int		check_pipe_fin(char *line); // verifier s'il y a un pipe a la fin de la chaine ou l'espace seulement apres le dernier pipe
int 	count_pipe(t_token *token); // compter le nombre de pipes dans la liste chainee

// free_parsing.c
void	free_tokens(t_token **token); // liste free
void	free_tab_char(char **tab); // free le tableau de string 
void	free_tab_int(int *tab); // free le tableau d'int
void	free_temp_heredoc(char **temp); // supprimer tous les fichiers temporaires de heredoc (unlink), puis free le tableau temp_heredoc[]
void	free_cmd_fd_tab(t_cmd *cmd); // free les fd de cmd, puis free les tableaux 
void  free_cmd_partiel(t_cmd *cmd, int nbr_cmd);
void	free_cmd_all(t_cmd *cmd, int nbr_cmd); // free tous les cmd 
void	free_mini(t_mini *mini);

// commande.c
void	init_cmd(t_cmd *cmd, int j); // initialiser la structure cmd
t_cmd	*malloc_cmd(t_token *token); // alluer la liste chainee cmd (divisee par pipe)
char	**add_double_tab_char(char **tab, char *str, int size); // agrandir un tableau et rajouter une chaine
int		*add_double_tab_int(int *tab, int val, int size); // agrandir un tableau int et rajouter une valeur int
int 	add_cmd(t_token *token, t_cmd *cmd); // parcours les token, et rajoute les token dans les tableaux
int 	appliquer_add_cmd_heredoc_all(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd);
int 	appliquer_add_cmd_mot_all(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd);
int 	appliquer_add_cmd_redir(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd);
void	init_var_cmd(t_var_cmd *var_cmd, int *resultat);

// quote_enlever.c
char	*enlever_quote_dans_token(char *str); // enlever les quotes dans un token str

// env_expansion.c
char	*get_env_name(char *str, int start); // recuperer le nom de la variable d'env apres $
char	*get_env_var(char *str, t_mini *mini); // recuperer $ env variable
char	*ajouter_char(char *resultat, char c); // ajouter un char c a la fin de la chaine resultat  
char	*appliquer_env_var(char *resultat, char *str, t_mini *mini, int *i); // appliquer la variable d'env dans str a la position i (qui est le $)
char	*remplacer_dollar(char *str, t_mini *mini); // remplacement de $ par la valeur de la variable d'env
int		appliquer_dollar_sur_liste_token(t_token **token, t_mini *mini); // appliquer le remplacement de $ sur toute la liste chainee token
int		appliquer_quote_sur_liste_token(t_token **token); // enlever des quotes pour chaque token de type T_MOT et fd redir

// redir_in_out.c
void	process_out_redir(t_mini *mini, int i); // proceder a la redirection de sortie pour la commande i (> ou >>)
int		appliquer_infile(t_mini *mini, int i); // appliquer la redirection infile (<) pour la commande i

// heredoc.c
int		preparer_temp_file_name(t_mini *mini, int j, int n);
int		collecter_heredoc_lines(int fd, t_mini *mini, int j, int n); // recuperer les lignes de heredoc, puis les stocker dans le fichier temp
void	appliquer_heredoc_enfant(t_mini *mini, int j, int n); // appliquer heredoc dans le processus enfant
int		check_quote_limiter(char *limiter); // verifier s'il y a une quote paire dans limiter
int		check_heredoc_env(char *limiter); // verifier soit on applique l'expansion de l'env ou non par rapport a limiter
int		appliquer_heredoc_cmd(t_mini *mini, int j);

// signaux.c
void	init_signaux(void); // gerer les sigaux (ctrl-C, ctrl-\)
void	appliquer_sigint_prompt(int sig); // gerer au cas de ctrl-C
void	print_heredoc_warning_ctrl_d(char *delimiter); // afficher le message d'erreur quand on saisit ctrl d dans heredoc 

// tester
const char	*get_token_type_str(t_type_token type); // pour tester (enum -> string)	
char		*get_token_type_state(t_state state); // pour tester type quote
void 		test_print_cmds(t_cmd *cmd, int nbr_cmd); // tester le contenu de chaque cmd
void		test_redirs(t_mini *mini);

#endif
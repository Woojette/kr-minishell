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

typedef struct s_token
{
	char			*str; // token = <<
	t_type_token	type_token; //= T_RD_HEREDOC
	t_type_bi		type_bi; // type builtin
	t_state			type_quote; // = GENERAL
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	**cmd;
}	t_cmd;

typedef struct s_mini
{
	char						**env;
	int							exit_status;
	struct s_token	*token;
	struct s_mini		*next;
}	t_mini;



// typedef struct s_parse
// {
// 	char			*val;
// 	t_state			state;
// 	struct s_parse	*next;
// }	t_parse;

// typedef struct minis
// {
// 	char	**res;
// 	char	*line;
// 	char	**env;
// 	int		exit_status;
// }	t_minis;


// en cours

void	parse_builtin(char *line);

// ======================================================= token =======================================================

// parsing
void	add_token(char *line, t_type_token type_token, int len, t_token **token); // ajouter des token dans la structure
void	parse_input(char *line, t_token **token); // mettre des token a chaque noeud (mot, redir, pipe) 
char	**split_input_par_pipe(char *line); // decouper des commandes par pipe
void 	parse_fd_tokens(t_token **token); // pour la condition de token MOT (redir, fd)

// tester
const char	*get_token_type_str(t_type_token type); // pour tester (enum -> string)	
char		*get_token_type_state(t_state state); // pour tester type quote
void test_print_cmds(t_cmd *cmd, int nbr_cmd); // tester le contenu de chaque cmd

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

// pour compter len type mot
int		len_mot_total(char *line); // compter len du type mot (avec quote + sans quote)
int		len_mot_avant_quote(char *line); // recuperer len avant la quote qui est au milieu de la chaine
int		len_mot_2_quotes_entier(char *line); // compter le nombre de caracteres entre 2 quotes, y compris les 2 quotes
int		len_mot_apres_quote(char *line); // recuperer len apres la 2e quote

// pas de quote dans la chaine
int		len_mot_sans_quote(char *line); // compter le nombre de caracteres s'il y a pas de 2 quotes qui fonctionnent

// pipe
int		check_pipe_fin(char *line); // verifier s'il y a un pipe a la fin de la chaine ou l'espace seulement apres le dernier pipe

// free
void	free_tokens(t_token **token); // liste free


// ======================================================= commande ===================================================

int 	count_pipe(t_token *token); // compter le nombre de pipes dans la liste chainee
t_cmd	*malloc_cmd(t_token *token); // alluer la liste chainee cmd (divisee par pipe)
char**	add_double_tab(char **tab, char *str, int size); // agrandir un tableau et rajouter une chaine
int 	add_cmd(t_token *token, t_cmd *cmd); // parcours les token, et rajoute les token dans les tableaux
int		decouper_cmd_par_pipe(t_token *token, t_cmd **cmd);


// ====================================================================================================================

// ========================================================= quote =====================================================

// dollar
char	*get_env_name(char *str, int start); // recuperer le nom de la variable d'env apres $
char	*get_env_var(char *str, char **env); // recuperer $ env variable
char	*ajouter_char(char *resultat, char c); // ajouter un char c a la fin de la chaine resultat  
char	*appliquer_env_var(char *resultat, char *str, t_token *token, int *i); // appliquer la variable d'env dans str a la position i (qui est le $)
char	*remplacer_dollar(char *str, t_token *token); // remplacement de $ par la valeur de la variable d'env

// void	ft_echo(char *str, int option_n);
// void	ft_env(char **env);
// int		ft_export_all(char **tab, char ***env);
// int		ft_check_env_egal(char *str);
// int		ft_check_env_double(char *str, char **env);
// int		ft_export_sans_double(char *str, char ***env);
// int		ft_export_double(char *str, char ***env);
// int		ft_export(char *str, char ***env);
// int		ft_unset_all(char **tab, char ***env);
// int		ft_unset(char *str, char ***env);
// void	ft_unset_init_int_zero(int *j, int *supprime, int *taille);
// int		ft_pwd(void);

#endif
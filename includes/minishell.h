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


typedef struct s_mini
{
	char			*cmd;
	char			*redir;
	char			*arg;
	int				arg_index;
	int				exit_status;
	// t_parse			parse;
	struct s_mini	*next;
}	t_mini;



// typedef struct s_parse
// {
// 	char			*val;
// 	t_state			state;
// 	struct s_parse	*next;
// }	t_parse;

typedef struct minis
{
	char	**res;
	char	*line;
	char	**env;
	int		exit_status;
}	t_minis;


// en cours

void	parse_builtin(char *line);

// ======================================================= token =======================================================

// parsing
void	add_token(char *line, t_type_token type_token, int len, t_token **token); // ajouter des token dans la structure
void	parse_input(char *line, t_token **token); // mettre des token a chaque noeud (mot, redir, pipe) 
char	**split_input_par_pipe(char *line); // decouper des commandes par pipe

// tester
const char	*get_token_type_str(t_type_token type); // pour tester (enum -> string)	
char		*get_token_type_state(t_state state); // pour tester type quote


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

// free
void	free_tokens(t_token **token); // liste free

// ====================================================================================================================

// pipe

int		check_pipe_fin(char *line); // verifier s'il y a un pipe a la fin de la chaine ou l'espace seulement apres le dernier pipe
void 	parse_fd_tokens(t_token **token); // pour la condition de token MOT (redir, fd)

// quote
int		appliquer_quote(t_token *token, char **env);
int		len_doller_espace(t_token *token, char c); // compter le nombre de caracteres (a partir de $ jusqu'a l'espace)
char	*get_env_var(char *str, char **env); // recuperer $ env variable


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
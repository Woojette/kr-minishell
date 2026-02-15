#ifndef EXEC_RUN_H
#define EXEC_RUN_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
# include "libft.h"

typedef struct s_cmd
{
	char	**cmd; // 이름 바꾸자고 건의하기..
	// cmd redir
	char	**infile; // fichier de redirection entree (<)
	char	**outfile; // fichier de redirection sortie (>)
	int		*out_append; // 1 si redirection en mode append (>>), 0 sinon
	char	*temp_heredoc;
	pid_t	pid_heredoc;
	int		heredoc; // 1 si redirection heredoc (<<), 0 sinon
	char	*limiter; // limiteur pour heredoc
	// resultat des fd
	int		fd_in; // resultat ouverture fichier de < or <<
	int		fd_out; // resultat ouverture fichier de > or >>
	// erreurs de redirection
	int		in_fail; // 1 si echec ouverture fichier de < or <<, 0 sinon
	int		out_fail; // 1 si echec ouverture fichier de > or >>, 0 sinon
}	t_cmd;

typedef struct s_mini
{
	int	    pipe_read_end;
	char	**env;
	char	**path_array; //split 결과 배열
	int		exit_status;
	t_cmd	*cmd_array; // tableau de structures cmd (divise par pipe) // 얘도 이름 바꾸자고..
	int		nbr_cmd; // nombre de commandes (nombre de structures cmd dans cmd_tab)
}	t_mini;



void  fork_center(t_mini *mini);
void	ft_execute(t_mini *mini, t_cmd *cmd);

t_mini *build_echo_cat_wc(void);

// path_utils.c
void	set_path_array(t_mini *mini);
char	*cmd_path_center(t_mini *mini, char *cmd);

// error_utils.c
void	fatal_error(t_mini *mini, char *err);

// file_utils.c
int	is_directory(char *file_path);
int	is_executable(char *file_path);
int	does_file_exist(char *file_path);

void	ft_close(t_mini* mini, int *fd);

#endif

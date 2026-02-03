#ifndef EXEC_RUN_H
#define EXEC_RUN_H

#include <sys/types.h>
#include <unistd.h>


typedef struct s_cmd
{
	char	**cmd;
	// cmd redir
	char	**infile; // fichier de redirection entree (<)
	char	**outfile; // fichier de redirection sortie (>)
	int		out_append; // 1 si redirection en mode append (>>), 0 sinon
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
	char	**env;
	int		exit_status;
	t_cmd	*cmd; // tableau de structures cmd (divise par pipe)
	int		nbr_cmd; // nombre de commandes (nombre de structures cmd dans cmd_tab)
}	t_mini;


t_mini *build_echo_cat_wc(void);

#endif

#include "minishell.h"

// gerer les sigaux (ctrl-C, ctrl-\)
void	init_signaux(void)
{
	signal(SIGINT, appliquer_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

// gerer au cas de ctrl-C
void	appliquer_sigint_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
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

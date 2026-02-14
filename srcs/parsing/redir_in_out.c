#include "minishell.h"

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
	if (!mini->cmd_array || !mini->cmd_array[i].inoutfile) // si cmd n'existe pas ou outfile est NULL
		return ;
	if (mini->cmd_array[i].out_fail || mini->cmd_array[i].inout_fail) // si deja echec de redir in ou out, on ne fait rien
		return ;
	while (mini->cmd_array[i].inoutfile[n]) // pour chaque fichier de redirection outfile
	{
		type_redir = 0; // a chaque iteration de outfile[n], type_redir commence toujours par 0 (> par defaut)
		fd_temp = -1; // on initialise a nouveau le fd temporaire a chaque iteration
		// si un tableau out_append existe, on recupere le type de redir associe a ce fichier
		// out_append[n] correspond a outfile[n] (0 est >, 1 est >>)
		if (!mini->cmd_array[i].out_append || n >= len_tab_char(mini->cmd_array[i].inoutfile))
			type_redir = 0; // s'il y a un probleme, 0 par defaut
		else
			type_redir = mini->cmd_array[i].out_append[n];
		if (type_redir == 1) // si out_append == 1, c'est une redirection en mode append (>>)
			fd_temp = open(mini->cmd_array[i].inoutfile[n], O_WRONLY | O_APPEND | O_CREAT, 0644);
		else // si out_append == 0, c'est une redirection simple (>)
			fd_temp = open(mini->cmd_array[i].inoutfile[n], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd_temp < 0)
		{
			perror(mini->cmd_array[i].inoutfile[n]); // afficher l'erreur
			mini->exit_status = 1; // mettre le code de sortie a 1
			mini->cmd_array[i].out_fail = 1; // marquer que l'ouverture a echoue
			if (mini->cmd_array[i].fd_out != -1)
			{
				close(mini->cmd_array[i].fd_out);
				mini->cmd_array[i].fd_out = -1;
			}
			return ;
		}
		if (mini->cmd_array[i].fd_out != -1) // si un ancien fichier out existe,
			close(mini->cmd_array[i].fd_out); // on le ferme avant de le remplacer
		mini->cmd_array[i].fd_out = fd_temp; // puis on en ajoute le nouveau
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
	if (!mini->cmd_array) // si cmd n'existe pas
		return (-1);
	if (mini->cmd_array[i].inout_fail || mini->cmd_array[i].out_fail) // si deja echec de redir in ou out, on ne fait rien
		return (0);
	if (mini->cmd_array[i].infile == NULL) // proteger au cas ou infile est NULL
		return (0);
	while (mini->cmd_array[i].infile[n]) // pour chaque fichier de redirection infile
	{
		fd_temp = open(mini->cmd_array[i].infile[n], O_RDONLY);
		// a chaque iteration de infile[n], ouvrir le fichier en lecture seule dans un fd temporaire
		if (fd_temp < 0) // si echec d'ouverture
		{
			perror(mini->cmd_array[i].infile[n]); // afficher l'erreur
			mini->exit_status = 1; // mettre le code de sortie a 1
			mini->cmd_array[i].inout_fail = 1; // marquer que l'ouverture a echoue
			if (mini->cmd_array[i].fd_in != -1)
			{
				close(mini->cmd_array[i].fd_in);
				mini->cmd_array[i].fd_in = -1;
			}
			return (-1);
		}
		if (mini->cmd_array[i].fd_in != -1) // si un ancien fichier in existe,
		{
			close(mini->cmd_array[i].fd_in); // on le ferme avant de le remplacer
			mini->cmd_array[i].fd_in = -1;
		}
		mini->cmd_array[i].fd_in = fd_temp; // reinitialiser fd_in
		// ce fichier devient l'entree active (le dernier redir qui va s'effectuer)
		n++;
	}
	return (0);
}

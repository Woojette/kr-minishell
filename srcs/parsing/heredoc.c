#include "minishell.h"

// Préparation du nom de fichier temporaire pour heredoc
// j: index de cmd
// n: index du heredoc pour cette cmd 
// ex) cmd1 << limiter1 << limiter2 -> n = 0 pour limiter1, n = 1 pour limiter2
int	preparer_temp_file_name(t_mini *mini, int j, int n)
{
	char	*index_char;
	char	*temp_j;
	char	*tiret_bas;
	char	*temp_j_n;

	if (!mini || j < 0 || j >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return (-1);
	if (n < 0 || n >= mini->cmd[j].compter_heredoc) // si n est invalide (ex. n = 2 alors que compter_heredoc = 2, donc les index valides sont 0 et 1)
		return (-1);
	if (mini->cmd[j].temp_heredoc && mini->cmd[j].temp_heredoc[n]) // si un ancien fichier temporaire existe deja pour ce heredoc[n], on le supprime
	{
		unlink(mini->cmd[j].temp_heredoc[n]); // supprimer l'ancien fichier temporaire s'il existe
		free(mini->cmd[j].temp_heredoc[n]); // free l'ancien nom de fichier temporaire
		mini->cmd[j].temp_heredoc[n] = NULL; // reinitialiser a NULL
	}
	index_char = ft_itoa(j); // convertir l'index j en string pour nommer l'index du fichier temporaire (temp_j)
	if (!index_char)
		return (perror("index j (itoa)"), -1);
	temp_j = ft_strjoin("temp_", index_char); // construire le nom du fichier temporaire pour heredoc: temp_j
	free(index_char); // liberer index_char apres utilisation
	if (!temp_j) // si echec de malloc pour le nom du fichier temporaire
		return (perror("strjoin: temp_ + j"), -1);
	tiret_bas = ft_strjoin(temp_j, "_"); // ajouter un tiret bas entre temp_j et n pour le nom du fichier temporaire (temp_j_n)
	free(temp_j); // liberer temp_j apres utilisation
	if (!tiret_bas)
		return (perror("strjoin: temp_j + _"), -1);
	index_char = ft_itoa(n); // l'index n en char pour le fichier temporaire (temp_j_n)
	if (!index_char)
		return (perror("itoa index n"), free(tiret_bas), -1);
	temp_j_n = ft_strjoin(tiret_bas, index_char); // construire le nom du fichier temporaire pour heredoc: temp_j_n
	free(tiret_bas); // liberer temp_j apres utilisation
	free(index_char); // liberer index_char apres utilisation
	if (!temp_j_n) // si echec de malloc pour le nom du fichier temporaire
		return (perror("malloc: file name j n"), -1);
	mini->cmd[j].temp_heredoc[n] = temp_j_n; // assigner le nom du fichier temporaire a mini->cmd[j].temp_heredoc
	if (access(mini->cmd[j].temp_heredoc[n], F_OK) == 0) // si le fichier mini->cmd[j].temp_heredoc existe deja
		unlink(mini->cmd[j].temp_heredoc[n]); // supprimer le fichier existant
	return (0);
}

// recuperer les lignes de heredoc, puis les stocker dans le fichier temp
// fd = fd de fichier temporaire temp, delimiter = limiter
// int	collecter_heredoc_lines(int fd, char *delimiter)
int	collecter_heredoc_lines(int fd, t_mini *mini, int j, int n)
{
	char	*line;
	char	*line_applique;

	if (fd < 0 || !mini || !mini->cmd
		|| j < 0 || j >= mini->nbr_cmd || !mini->cmd[j].limiter
		|| n < 0 || n >= mini->cmd[j].compter_heredoc)
		return (-1);
	if (!mini->cmd[j].limiter[n])
		return (1);
	while (1)
	{
		line = readline("> "); // afficher un prompte qui ressemble a heredoc
		if (!line) // saisit ctrl+D -> on quitte
			return (1); // dans ce cas (ctrl+D), pas besoin de liberer le memoire, puisqu'il y en a pas
		if (mini->cmd[j].limiter[n] && ft_strcmp(line, mini->cmd[j].limiter[n]) == 0) // quand on croise limiter -> on quitte
		// si delimiter est NULL -> erreur (pour proteger on ajoute dans la condition delimiter aussi)
		{
			free(line); // liberer readline
			return (0); // quitte la boucle (et cette fonction)
		}
		line_applique = line;
		if (mini->cmd[j].hd_env && mini->cmd[j].hd_env[n])
		{
			line_applique = remplacer_dollar(line, mini);
			if (!line_applique)
				return (free(line), -1);
		}
		if (write(fd, line_applique, ft_strlen(line_applique)) == -1 || write(fd, "\n", 1) == -1) // ecrire la ligne dans le fichier temp (le resultat de line + '\n')
		// le resultat de readline n'applique pas automatiquement '\n', on en ajoute a la fin
		// si write retourne -1, c'est une erreur d'ecriture, on libere readline et on retourne -1
		{
			if (mini->cmd[j].hd_env && mini->cmd[j].hd_env[n])
				free(line_applique);
			free(line);
			return (-1); // si echec d'ecriture
		}
		if (mini->cmd[j].hd_env && mini->cmd[j].hd_env[n])
			free(line_applique);
		free(line); // free readline avant de quitter la fonction hihi
	}
	return (0);
}

// appliquer heredoc dans le processus enfant
// le processus enfant: ecrire les lignes du heredoc n dans le fichier temporaire temp_heredoc[n] jusqu'a ce qu'on arrive a limiter[n], puis quitter
void	appliquer_heredoc_enfant(t_mini *mini, int j, int n)
{
	int	resultat;
	int	fd_temp;

	signal(SIGINT, SIG_DFL); // quand on saisit ctrl-C, le processus enfant doit etre termine (ctrl-C doit finir seulement le processus enfant)
	signal(SIGQUIT, SIG_IGN); // quand on saisit ctrt-\, on l'ignore (ca change rien)
	if (!mini || j < 0 || j >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		exit (1);
	if (n < 0 || n >= mini->cmd[j].compter_heredoc) // si n est invalide (ex. n = 2 alors que compter_heredoc = 2, donc les index valides sont 0 et 1)
		exit (1);
	if (!mini->cmd[j].temp_heredoc || !mini->cmd[j].temp_heredoc[n]) // si le nom du fichier n'existe pas, on finit
		exit (1);
	fd_temp = open(mini->cmd[j].temp_heredoc[n], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// ouvrir (créer) le fichier temporaire en écriture (on ecrase le contenu prcedent)
	if (fd_temp == -1)
	{
		perror("open temp");
		exit (1);
	}
	if (!mini->cmd[j].limiter || !mini->cmd[j].limiter[n]) // si limiter est NULL, c'est une erreur, on finit
	{
		close(fd_temp);
		exit (1);
	}
	resultat = collecter_heredoc_lines(fd_temp, mini, j, n);
	// collecter des lignes heredoc dans le fichier temp
	// (lire des lignes jusqu'a ce qu'on arrive limiter[n], puis les ecrire dans le fichier temp)
	if (resultat == 1) // ctrl-D
	{
		print_heredoc_warning_ctrl_d(mini->cmd[j].limiter[n]);
		close(fd_temp);
		exit (0);
	}
	if (resultat < 0) // erreur dans collecter_heredoc_lines
	{
		perror("collecter heredoc: mini->cmd[j].temp_heredoc[n]");
		close(fd_temp);
		unlink(mini->cmd[j].temp_heredoc[n]);
		exit (1);
	}
	close(fd_temp); //fermer le fd temp dans l'enfant pour eviter les leak
	exit(0);
}

// verifier s'il y a une quote paire dans limiter
// 1: on trouve au moins une quote bien fermee dans limiter
// 0: aucune quote paire ou pas de quote
int	check_quote_limiter(char *limiter)
{
	int		i;
	char	quote;

	i = 0;
	if (!limiter)
		return (0);
	while (limiter[i] != '\0')
	{
		if (limiter[i] == '\'' || limiter[i] == '"') // si on croise une quote
		{
			quote = limiter[i]; // sauvegarder la premiere quote
			i++; // avancer pour chercher la quote fermante
			while (limiter[i] && limiter[i] != quote) // tout ce qui n'est pas la meme quote, on passe
				i++;
			if (limiter[i] == quote) // 1 si on trouve la meme quote paire
				return (1);
			else
				return (0); // 0 quote ouverte mais pas fermee
		}
		i++;
	}
	return (0); // pas de quote
}

// verifier soit on applique l'expansion de l'env ou non par rapport a limiter
int	check_heredoc_env(char *limiter)
{
	if (!limiter)
		return (0);
	if (!check_quote_limiter(limiter))
		return (1); // 1 s'il y a pas de quote dans limiter (on applique l'expansion de l'env)
	return (0); // 0 sinon (on n'applique pas l'expansion de l'env, on les lit comme un caractere)
}

// appliquer heredoc pour la commande j
int	appliquer_heredoc_cmd(t_mini *mini, int j)
{
	int	status; // pour waitpid (si le processus enfant s'est termine correctement)
	int	exit_status; // pour resultat du waitpid (code de sortie du processus enfant)
	int	exit_signal;
	int	n;

	n = 0;
	if (!mini || j < 0 || j >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return (-1);
	if (mini->cmd[j].compter_heredoc <= 0) // si il n'y a pas de heredoc pour cette cmd, on ne fait rien
		return (0);

	signal(SIGINT, SIG_IGN); // le processus parent ignore ctrl+C pendant le processus enfant (heredoc fork)
	signal(SIGQUIT, SIG_IGN); // le processus parent ignore ctrl+\ pendant le processus enfant (heredoc fork)

	if (mini->cmd[j].temp_heredoc == NULL) // il faut d'abord allouer le tableau temp_heredoc[] pour stocker les noms des fichiers temporaires de chaque heredoc, avant de remplir temp_heredoc[n]
	{
		mini->cmd[j].temp_heredoc = ft_calloc(mini->cmd[j].compter_heredoc + 1, sizeof(char *)); // +1 pour NULL (terminaison)
		if (!mini->cmd[j].temp_heredoc)
		{
			mini->cmd[j].inout_fail = 1;
			mini->exit_status = 1;
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
	}
	while (n < mini->cmd[j].compter_heredoc) // pour chaque heredoc de cette cmd, on prepare le nom du fichier temporaire
	{
		if (preparer_temp_file_name(mini, j, n) == -1)
		{
			mini->cmd[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd[j].temp_heredoc);
			mini->cmd[j].temp_heredoc = NULL;
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
		mini->cmd[j].pid_heredoc = fork(); // creer un processus enfant pour gerer heredoc
		if (mini->cmd[j].pid_heredoc == -1) // si echec de fork
		{
			mini->cmd[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd[j].temp_heredoc);
			mini->cmd[j].temp_heredoc = NULL;
			init_signaux();
			return (-1); // si echec de waitpid
		}
		if (mini->cmd[j].pid_heredoc == 0) // processus enfant
			appliquer_heredoc_enfant(mini, j, n);
		if (waitpid(mini->cmd[j].pid_heredoc, &status, 0) == -1) // attendre la fin du processus enfant
		{
			init_signaux();
			mini->cmd[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd[j].temp_heredoc);
			mini->cmd[j].temp_heredoc = NULL;
			return (-1); // si echec de waitpid
		}
		// init_signaux(); // apres la fin du processus enfant, on applique des signaux pareils que shell
		if (WIFSIGNALED(status))
		{
			exit_signal = WTERMSIG(status);
			mini->cmd[j].inout_fail = 1;
			mini->exit_status = 128 + exit_signal;
			free_temp_heredoc(mini->cmd[j].temp_heredoc);
			mini->cmd[j].temp_heredoc = NULL;
			init_signaux();
			return (-1);
		}
		else if (WIFEXITED(status)) // si le processus enfant s'est termine correctement
		{
			exit_status = WEXITSTATUS(status); // recuperer le code de sortie
			if (exit_status != 0) // si le code de sortie n'est pas 0 (erreur dans heredoc)
			{
				mini->cmd[j].inout_fail = 1; // marquer l'echec de heredoc
				mini->exit_status = exit_status; // mettre a jour le code de sortie global
				free_temp_heredoc(mini->cmd[j].temp_heredoc);
				mini->cmd[j].temp_heredoc = NULL;
				init_signaux(); // reinitialiser les signaux avant de retourner
				return (-1); // retourner -1 pour indiquer l'erreur
			}
		}
		else // si le processus enfant ne s'est pas termine correctement
		{
			mini->cmd[j].inout_fail = 1; // marquer l'echec de heredoc
			mini->exit_status = 1; // mettre a jour le code de sortie global
			free_temp_heredoc(mini->cmd[j].temp_heredoc);
			mini->cmd[j].temp_heredoc = NULL;
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
		if (!mini->cmd[j].temp_heredoc || !mini->cmd[j].temp_heredoc[n]) // proteger au cas ou temp_heredoc est NULL
		{
			mini->cmd[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd[j].temp_heredoc);
			mini->cmd[j].temp_heredoc = NULL;
			init_signaux(); // reinitialiser les signaux avant de retourner
			return (-1);
		}
		n++;
	}
	if (mini->cmd[j].fd_in != -1) // si fd_in est deja ouvert, on le ferme d'abord
	{
		close(mini->cmd[j].fd_in); // fermer l'ancien fd_in avant de le remplacer
		mini->cmd[j].fd_in = -1;
	}
	mini->cmd[j].fd_in = open(mini->cmd[j].temp_heredoc[mini->cmd[j].compter_heredoc - 1], O_RDONLY); // reouvrir le fichier temp en lecture seule
	if (mini->cmd[j].fd_in == -1) // si echec d'ouverture de temp en lecture
	{
		mini->cmd[j].inout_fail = 1;
		mini->exit_status = 1;
		perror("open temp for reading"); // afficher l'erreur
		free_temp_heredoc(mini->cmd[j].temp_heredoc);
		mini->cmd[j].temp_heredoc = NULL;
		init_signaux(); // reinitialiser les signaux avant de retourner
		return (-1);
	}
	init_signaux(); // reinitialiser les signaux avant de retourner
	return (0);
}

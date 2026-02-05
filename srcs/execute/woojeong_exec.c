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
	if (!mini->cmd || !mini->cmd[i].outfile) // si cmd n'existe pas ou outfile est NULL
		return ;
	if (mini->cmd[i].out_fail || mini->cmd[i].in_fail) // si deja echec de redir in ou out, on ne fait rien
		return ;
	while (mini->cmd[i].outfile[n]) // pour chaque fichier de redirection outfile
	{
		type_redir = 0; // a chaque iteration de outfile[n], type_redir commence toujours par 0 (> par defaut)
		fd_temp = -1; // on initialise a nouveau le fd temporaire a chaque iteration
		// si un tableau out_append existe, on recupere le type de redir associe a ce fichier
		// out_append[n] correspond a outfile[n]
		// -1 signifie que c'est pas defini, donc on garde d'abord le type de redir par defaut (>)
		if (mini->cmd[i].out_append && mini->cmd[i].out_append[n] != -1)
			type_redir = mini->cmd[i].out_append[n];
		if (type_redir == 1) // si out_append == 1, c'est une redirection en mode append (>>)
			fd_temp = open(mini->cmd[i].outfile[n], O_WRONLY | O_APPEND | O_CREAT, 0644);
		else // si out_append == 0, c'est une redirection simple (>)
			fd_temp = open(mini->cmd[i].outfile[n], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd_temp < 0)
		{
			perror(mini->cmd[i].outfile[n]); // afficher l'erreur
			mini->exit_status = 1; // mettre le code de sortie a 1
			mini->cmd[i].out_fail = 1; // marquer que l'ouverture a echoue
			return ;
		}
		if (mini->cmd[i].fd_out != -1) // si un ancien fichier out existe,
			close(mini->cmd[i].fd_out); // on le ferme avant de le remplacer
		mini->cmd[i].fd_out = fd_temp; // puis on en ajoute le nouveau
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
	if (!mini->cmd) // si cmd n'existe pas
		return (-1);
	if (mini->cmd[i].in_fail || mini->cmd[i].out_fail) // si deja echec de redir in ou out, on ne fait rien
		return (0);
	if (mini->cmd[i].infile == NULL) // proteger au cas ou infile est NULL
	{
		mini->exit_status = 2;
		return (-1);
	}
	while (mini->cmd[i].infile[n]) // pour chaque fichier de redirection infile
	{
		fd_temp = open(mini->cmd[i].infile[n], O_RDONLY);
		// a chaque iteration de infile[n], ouvrir le fichier en lecture seule dans un fd temporaire
		if (fd_temp < 0) // si echec d'ouverture
		{
			perror(mini->cmd[i].infile[n]); // afficher l'erreur
			mini->exit_status = 1; // mettre le code de sortie a 1
			mini->cmd[i].fd_in = -1; // marquer que l'ouverture a echoue
			mini->cmd[i].in_fail = 1; // marquer que l'ouverture a echoue
			return (-1);
		}
		if (mini->cmd[i].fd_in != -1) // si un ancien fichier in existe,
			close(mini->cmd[i].fd_in); // on le ferme avant de le remplacer
		mini->cmd[i].fd_in = fd_temp; // reinitialiser fd_in
		// ce fichier devient l'entree active (le dernier redir qui va s'effectuer)
		n++;
	}
	return (0);
}

// Préparation du nom de fichier temporaire pour heredoc
int	preparer_temp_file_name(t_mini *mini, int i)
{
	char	*temp_index; // pour faire l'etiquette de l'index du fichier temporaire

	if (!mini || i < 0 || i >= mini->nbr_cmd) // si mini n'existe pas, index i est invalide
		return (-1);
	// if (mini->cmd[i].fd_in != -1) // si fd_in est deja ouvert, on le ferme d'abord
	// 	close (mini->cmd[i].fd_in); // fermer l'ancien descripteur de fichier
	if (mini->cmd[i].temp_heredoc)
	{
		unlink(mini->cmd[i].temp_heredoc); // supprimer l'ancien fichier temporaire s'il existe
		free(mini->cmd[i].temp_heredoc); // free l'ancien nom de fichier temporaire
		mini->cmd[i].temp_heredoc = NULL; // reinitialiser a NULL
	}
	temp_index = ft_itoa(i); // convertir l'index i en string pour nommer l'index du fichier temporaire
	if (!temp_index)
	{
		// mini->cmd[i].in_fail = 1;
		perror("malloc: temp index");
		return (-1);
	}
	mini->cmd[i].temp_heredoc = ft_strjoin("temp_", temp_index); // construire nom du fichier temporaire pour heredoc: temp_i
	free(temp_index); // liberer temp_index apres utilisation
	if (!mini->cmd[i].temp_heredoc) // si echec de malloc pour le nom du fichier temporaire
	{
		// mini->cmd[i].in_fail = 1;
		perror("malloc: temp heredoc name");
		return (-1);
	}
	if (access(mini->cmd[i].temp_heredoc, F_OK) == 0 && unlink(mini->cmd[i].temp_heredoc) == -1) // si le fichier mini->cmd[i].temp_heredoc existe deja
		perror("unlink temp heredoc"); // supprimer le fichier existant
	return (0);
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

// recuperer les lignes de heredoc, puis les stocker dans le fichier temp
// fd = fd de fichier temporaire temp, delimiter = limiter
int	collecter_heredoc_lines(int fd, char *delimiter)
{
	char *line;
	
	while (1)
	{
		line = readline("> "); // afficher un prompte qui ressemble a heredoc
		if (!line) // saisit ctrl+D -> on quitte
			return (1); // dans ce cas (ctrl+D), pas besoin de liberer le memoire, puisqu'il y en a pas
		if (delimiter && ft_strcmp(line, delimiter) == 0) // quand on croise limiter -> on quitte
		// si delimiter est NULL -> erreur (pour proteger on ajoute dans la condition delimiter aussi)
		{
			// if (!line) // ctrl d message d'erreur a faire apres *************************
			// 	return;
			free(line); // liberer readline
			return (0); // quitte la boucle (et cette fonction)
		}
		write(fd, line, ft_strlen(line)); // le resultat
		write(fd, "\n", 1); // vu que readline n'applique pas automatiquement '\n', on en ajoute a la fin
		free(line); // free readline avant de quitter la fonction hihi
	}
	return (0);
}

// appliquer heredoc dans le processus enfant
void	appliquer_heredoc_enfant(t_mini *mini, int i)
{
	int	resultat;
	int	fd_temp;

	signal(SIGINT, SIG_DFL); // quand on saisit ctrl+C, le processus enfant doit etre termine
	signal(SIGQUIT, SIG_IGN); // quand on saisit ctrt+\, on l'ignore (ca change rien)
	if (!mini->cmd[i].temp_heredoc) // si le nom du fichier n'existe pas, on finit
		exit (1);
	fd_temp = open(mini->cmd[i].temp_heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// ouvrir (créer) le fichier temporaire en écriture (on ecrase le contenu prcedent)
	if (fd_temp == -1)
	{
		perror("open temp");
		exit (1);
	}
	resultat = collecter_heredoc_lines(fd_temp, mini->cmd[i].limiter);
	// collecter des lignes heredoc dans le fichier temp
	// (lire des lignes jusqu'a ce qu'on arrive limiter, puis les ecrire dans le fichier temp)
	if (resultat == 1) // ctrl-D
	{
		print_heredoc_warning_ctrl_d(mini->cmd[i].limiter);
		close(fd_temp);
		exit (0);
	}
	close(fd_temp); //fermer le fd temp dans l'enfant pour eviter les leak
	exit(0);
}

// gerer au cas de ctrl-C
void	appliquer_sigint_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

// gerer les sigaux (ctrl-C, ctrl-\)
void	init_signaux(void)
{
	signal(SIGINT, appliquer_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

// appliquer heredoc pour la commande i
int	appliquer_heredoc_cmd(t_mini *mini, int i)
{
	int	status; // pour waitpid (si le processus enfant s'est termine correctement)
	int	exit_status; // pour resultat du waitpid (code de sortie du processus enfant)
	int	exit_signal;

	if (preparer_temp_file_name(mini, i) == -1)
	{
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 1;
		return (-1);
	}
	signal(SIGINT, SIG_IGN); // le processus parent ignore ctrl+C pendant le processus enfant (heredoc fork)
	signal(SIGQUIT, SIG_IGN);
	mini->cmd[i].pid_heredoc = fork(); // creer un processus enfant pour gerer heredoc
	if (mini->cmd[i].pid_heredoc == -1) // si echec de fork
	{
		init_signaux();
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 1;
		return (-1); // si echec de waitpid
	}
	if (mini->cmd[i].pid_heredoc == 0) // processus enfant
		appliquer_heredoc_enfant(mini, i);
	if (waitpid(mini->cmd[i].pid_heredoc, &status, 0) == -1) // attendre la fin du processus enfant
	{
		init_signaux();
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 1;
		return (-1); // si echec de waitpid
	}
	init_signaux(); // apres la fin du processus enfant, on applique des signaux pareils que shell
	if (WIFSIGNALED(status))
	{
		exit_signal = WTERMSIG(status);
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 128 + exit_signal;
		return (-1);
	}
	else if (WIFEXITED(status)) // si le processus enfant s'est termine correctement
	{
		exit_status = WEXITSTATUS(status); // recuperer le code de sortie
		if (exit_status != 0) // si le code de sortie n'est pas 0 (erreur dans heredoc)
		{
			mini->cmd[i].in_fail = 1; // marquer l'echec de heredoc
			mini->exit_status = exit_status; // mettre a jour le code de sortie global
			return (-1); // retourner -1 pour indiquer l'erreur
		}
	}
	else // si le processus enfant ne s'est pas termine correctement
	{
		mini->cmd[i].in_fail = 1; // marquer l'echec de heredoc
		mini->exit_status = 1; // mettre a jour le code de sortie global
		return (-1);
	}
	if (!mini->cmd[i].temp_heredoc) // proteger au cas ou temp_heredoc est NULL
	{
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 1;
		return (-1);
	}
	if (mini->cmd[i].fd_in != -1) // si fd_in est deja ouvert, on le ferme d'abord
		close(mini->cmd[i].fd_in); // fermer l'ancien fd_in avant de le remplacer
	mini->cmd[i].fd_in = open(mini->cmd[i].temp_heredoc, O_RDONLY); // reouvrir le fichier temp en lecture seule
	if (mini->cmd[i].fd_in == -1) // si echec d'ouverture de temp en lecture
	{
		mini->cmd[i].in_fail = 1;
		mini->exit_status = 1;
		perror("open temp for reading"); // afficher l'erreur
		return (-1);
	}
	return (0);
}


// juste pour tester les redir 
static void	print_preview_path(const char *path)
{
	int		fd;
	char	buf[256];
	ssize_t	n;

	if (!path)
		return ;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("preview open");
		return ;
	}
	n = read(fd, buf, sizeof(buf) - 1);
	if (n > 0)
	{
		buf[n] = '\0';
		printf("    [preview] ----\n%s\n    ------------\n", buf);
	}
	else
		printf("    [preview] (empty)\n");
	close(fd);
}

// ca aussi juste pour tester redir j'en ai maaaaaare
void	test_redirs(t_mini *mini)
{
	int	i;

	if (!mini || !mini->cmd)
		return ;

	i = 0;
	while (i < mini->nbr_cmd)
	{
		printf("\n========== CMD %d ==========\n", i);

		printf("raw: heredoc=%d limiter=%s temp=%s\n",
			mini->cmd[i].heredoc,
			mini->cmd[i].limiter ? mini->cmd[i].limiter : "(null)",
			mini->cmd[i].temp_heredoc ? mini->cmd[i].temp_heredoc : "(null)");

		printf("raw: infile=");
		if (mini->cmd[i].infile)
		{
			int k = 0;
			while (mini->cmd[i].infile[k])
			{
				printf("[%s] ", mini->cmd[i].infile[k]);
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		printf("raw: outfile=");
		if (mini->cmd[i].outfile)
		{
			int k = 0;
			while (mini->cmd[i].outfile[k])
			{
				int mode = 0;
				if (mini->cmd[i].out_append && mini->cmd[i].out_append[k] != -1)
					mode = mini->cmd[i].out_append[k];
				printf("[%s mode=%s] ", mini->cmd[i].outfile[k], mode ? ">>" : ">");
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		// 1) heredoc
		if (mini->cmd[i].heredoc)
		{
			if (appliquer_heredoc_cmd(mini, i) == -1)
				printf("    heredoc: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    heredoc: OK (fd_in=%d)\n", mini->cmd[i].fd_in);

			// heredoc lire
			if (!mini->cmd[i].in_fail && mini->cmd[i].temp_heredoc)
				print_preview_path(mini->cmd[i].temp_heredoc);
		}

		// 2) infile (s'il y a heredoc, on passe)
		if (!mini->cmd[i].heredoc && mini->cmd[i].infile && mini->cmd[i].infile[0])
		{
			if (appliquer_infile(mini, i) == -1)
				printf("    infile: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    infile: OK (fd_in=%d)\n", mini->cmd[i].fd_in);

			// infile: lire le dernier infile
			if (!mini->cmd[i].in_fail)
			{
				int last = 0;
				while (mini->cmd[i].infile[last])
					last++;
				if (last > 0)
					print_preview_path(mini->cmd[i].infile[last - 1]);
			}
		}

		// 3) outfile
		if (mini->cmd[i].outfile && mini->cmd[i].outfile[0])
		{
			process_out_redir(mini, i);
			if (mini->cmd[i].out_fail)
				printf("    outfile: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    outfile: OK (fd_out=%d)\n", mini->cmd[i].fd_out);
		}

		// resume
		printf("[summary] fd_in=%d fd_out=%d in_fail=%d out_fail=%d exit_status=%d\n",
			mini->cmd[i].fd_in, mini->cmd[i].fd_out,
			mini->cmd[i].in_fail, mini->cmd[i].out_fail, mini->exit_status);

		i++;
	}
	printf("\n============================\n");
}

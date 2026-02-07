#include "minishell.h"

// tester le contenu de chaque cmd
void test_print_cmds(t_cmd *cmd, int nbr_cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < nbr_cmd)
	{
		printf("command%d:\n", i);
		if (cmd[j].cmd == NULL)
			printf("cmd: NULL\n");
		else
		{
			while (cmd[j].cmd && cmd[j].cmd[i] != NULL)
			{
				printf("arg %d: %s\n", j, cmd[j].cmd[i]);
				i++;
			}
			j++;
			i = 0;
		}
	}
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

static void	print_in_redir_order(t_cmd *cmd)
{
	int k;

	if (!cmd)
		return ;

	printf("order: compter_in_hd=%d\n", cmd->compter_in_hd);

	printf("order: in_heredoc=");
	if (!cmd->in_heredoc)
		printf("(null)\n");
	else
	{
		k = 0;
		while (k < cmd->compter_in_hd)
		{
			printf("%d ", cmd->in_heredoc[k]); // 0=< , 1=<<
			k++;
		}
		printf("\n");
	}

	printf("order: in_hd_index=");
	if (!cmd->in_hd_index)
		printf("(null)\n");
	else
	{
		k = 0;
		while (k < cmd->compter_in_hd)
		{
			printf("%d ", cmd->in_hd_index[k]); // index dans infile[] ou limiter[]
			k++;
		}
		printf("\n");
	}
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

		// 0) dump parsing infos (infile/limiter/temp/outfile)
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

		printf("raw: limiter=");
		if (mini->cmd[i].limiter)
		{
			int k = 0;
			while (mini->cmd[i].limiter[k])
			{
				printf("[<< %s] ", mini->cmd[i].limiter[k]);
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		printf("raw: temp_heredoc=");
		if (mini->cmd[i].temp_heredoc)
		{
			int k = 0;
			while (mini->cmd[i].temp_heredoc[k])
			{
				printf("[%s] ", mini->cmd[i].temp_heredoc[k]);
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
				if (mini->cmd[i].out_append)
					mode = mini->cmd[i].out_append[k];
				printf("[%s mode=%s] ", mini->cmd[i].outfile[k], mode ? ">>" : ">");
				k++;
			}
		}
		else
			printf("(null)");
		printf("\n");

		// NEW: print in redir order arrays
		print_in_redir_order(&mini->cmd[i]);

		// 1) appliquer "in" redirections selon l'ordre (0=< , 1=<<)
		//    (pour tester: on simule juste l'effet final -> dernier redir gagne)
		if (mini->cmd[i].compter_in_hd > 0 && !mini->cmd[i].in_fail)
		{
			int k = 0;
			while (k < mini->cmd[i].compter_in_hd)
			{
				int kind = mini->cmd[i].in_heredoc ? mini->cmd[i].in_heredoc[k] : -1;
				int idx  = mini->cmd[i].in_hd_index ? mini->cmd[i].in_hd_index[k] : -1;

				if (kind == 0) // '<' -> infile[idx]
				{
					if (!mini->cmd[i].infile || idx < 0 || !mini->cmd[i].infile[idx])
						printf("    [in order] k=%d: <  idx=%d  (INVALID)\n", k, idx);
					else
						printf("    [in order] k=%d: <  idx=%d  file=%s\n", k, idx, mini->cmd[i].infile[idx]);
				}
				else if (kind == 1) // '<<' -> limiter[idx]
				{
					if (!mini->cmd[i].limiter || idx < 0 || !mini->cmd[i].limiter[idx])
						printf("    [in order] k=%d: << idx=%d  (INVALID)\n", k, idx);
					else
						printf("    [in order] k=%d: << idx=%d  limiter=%s\n", k, idx, mini->cmd[i].limiter[idx]);
				}
				else
					printf("    [in order] k=%d: kind=%d idx=%d (UNKNOWN)\n", k, kind, idx);

				k++;
			}
		}

		// 2) heredoc (si parsing a detecte au moins un <<)
		if (mini->cmd[i].compter_heredoc > 0)
		{
			if (appliquer_heredoc_cmd(mini, i) == -1)
				printf("    heredoc: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    heredoc: OK (fd_in=%d)\n", mini->cmd[i].fd_in);

			// preview last heredoc temp file (celui qui sera utilisé si dernier in redir est <<)
			if (!mini->cmd[i].in_fail && mini->cmd[i].temp_heredoc)
			{
				int last = 0;
				while (mini->cmd[i].temp_heredoc[last])
					last++;
				if (last > 0)
					print_preview_path(mini->cmd[i].temp_heredoc[last - 1]);
			}
		}

		// 3) infile (pour tester: on l'applique aussi, mais en vrai ça dépend de l'ordre)
		if (mini->cmd[i].infile && mini->cmd[i].infile[0])
		{
			if (appliquer_infile(mini, i) == -1)
				printf("    infile: FAIL (exit_status=%d)\n", mini->exit_status);
			else
				printf("    infile: OK (fd_in=%d)\n", mini->cmd[i].fd_in);

			// preview last infile
			if (!mini->cmd[i].in_fail)
			{
				int last = 0;
				while (mini->cmd[i].infile[last])
					last++;
				if (last > 0)
					print_preview_path(mini->cmd[i].infile[last - 1]);
			}
		}

		// 4) outfile
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

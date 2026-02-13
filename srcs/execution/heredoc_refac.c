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

	if (!mini || j < 0 || j >= mini->nbr_cmd) 
		return (-1);
	if (n < 0 || n >= mini->cmd_array[j].compter_heredoc) 
		return (-1);
	if (mini->cmd_array[j].temp_heredoc && mini->cmd_array[j].temp_heredoc[n]) 
	{
		unlink(mini->cmd_array[j].temp_heredoc[n]); 
		free(mini->cmd_array[j].temp_heredoc[n]); 
		mini->cmd_array[j].temp_heredoc[n] = NULL; 
	}
	index_char = ft_itoa(j); 
	if (!index_char)
		return (perror("index j (itoa)"), -1);
	temp_j = ft_strjoin("temp_", index_char); 
	free(index_char); 
	if (!temp_j) 
		return (perror("strjoin: temp_ + j"), -1);
	tiret_bas = ft_strjoin(temp_j, "_"); 
	free(temp_j); 
	if (!tiret_bas)
		return (perror("strjoin: temp_j + _"), -1);
	index_char = ft_itoa(n); 
	if (!index_char)
		return (perror("itoa index n"), free(tiret_bas), -1);
	temp_j_n = ft_strjoin(tiret_bas, index_char); 
	free(tiret_bas); 
	free(index_char); 
	if (!temp_j_n) 
		return (perror("malloc: file name j n"), -1);
	mini->cmd_array[j].temp_heredoc[n] = temp_j_n; 
	if (access(mini->cmd_array[j].temp_heredoc[n], F_OK) == 0) 
		unlink(mini->cmd_array[j].temp_heredoc[n]); 
	return (0);
}




int	collecter_heredoc_lines(int fd, t_mini *mini, int j, int n)
{
	char	*line;
	char	*line_applique;

	if (fd < 0 || !mini || !mini->cmd_array
		|| j < 0 || j >= mini->nbr_cmd || !mini->cmd_array[j].limiter
		|| n < 0 || n >= mini->cmd_array[j].compter_heredoc)
		return (-1);
	if (!mini->cmd_array[j].limiter[n])
		return (1);
	while (1)
	{
		line = readline("> "); 
		if (!line) 
			return (1); 
		if (mini->cmd_array[j].limiter[n] && ft_strcmp(line, mini->cmd_array[j].limiter[n]) == 0) 
		
		{
			free(line); 
			return (0); 
		}
		line_applique = line;
		if (mini->cmd_array[j].hd_env && mini->cmd_array[j].hd_env[n])
		{
			line_applique = remplacer_dollar(line, mini);
			if (!line_applique)
				return (free(line), -1);
		}
		if (write(fd, line_applique, ft_strlen(line_applique)) == -1 || write(fd, "\n", 1) == -1) 
		
		
		{
			if (mini->cmd_array[j].hd_env && mini->cmd_array[j].hd_env[n])
				free(line_applique);
			free(line);
			return (-1); 
		}
		if (mini->cmd_array[j].hd_env && mini->cmd_array[j].hd_env[n])
			free(line_applique);
		free(line); 
	}
	return (0);
}



void	appliquer_heredoc_enfant(t_mini *mini, int j, int n)
{
	int	resultat;
	int	fd_temp;

	signal(SIGINT, SIG_DFL); 
	signal(SIGQUIT, SIG_IGN); 
	if (!mini || j < 0 || j >= mini->nbr_cmd) 
		exit (1);
	if (n < 0 || n >= mini->cmd_array[j].compter_heredoc) 
		exit (1);
	if (!mini->cmd_array[j].temp_heredoc || !mini->cmd_array[j].temp_heredoc[n]) 
		exit (1);
	fd_temp = open(mini->cmd_array[j].temp_heredoc[n], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	if (fd_temp == -1)
	{
		perror("open temp");
		exit (1);
	}
	if (!mini->cmd_array[j].limiter || !mini->cmd_array[j].limiter[n]) 
	{
		close(fd_temp);
		exit (1);
	}
	resultat = collecter_heredoc_lines(fd_temp, mini, j, n);
	
	
	if (resultat == 1) 
	{
		print_heredoc_warning_ctrl_d(mini->cmd_array[j].limiter[n]);
		close(fd_temp);
		exit (0);
	}
	if (resultat < 0) 
	{
		perror("collecter heredoc: mini->cmd_array[j].temp_heredoc[n]");
		close(fd_temp);
		unlink(mini->cmd_array[j].temp_heredoc[n]);
		exit (1);
	}
	close(fd_temp); 
	exit(0);
}




int	check_quote_limiter(char *limiter)
{
	int		i;
	char	quote;

	i = 0;
	if (!limiter)
		return (0);
	while (limiter[i] != '\0')
	{
		if (limiter[i] == '\'' || limiter[i] == '"') 
		{
			quote = limiter[i]; 
			i++; 
			while (limiter[i] && limiter[i] != quote) 
				i++;
			if (limiter[i] == quote) 
				return (1);
			else
				return (0); 
		}
		i++;
	}
	return (0); 
}


int	check_heredoc_env(char *limiter)
{
	if (!limiter)
		return (0);
	if (!check_quote_limiter(limiter))
		return (1); 
	return (0); 
}


int	appliquer_heredoc_cmd(t_mini *mini, int j)
{
	int	status; 
	int	exit_status; 
	int	exit_signal;
	int	n;

	n = 0;
	if (!mini || j < 0 || j >= mini->nbr_cmd) 
		return (-1);
	if (mini->cmd_array[j].compter_heredoc <= 0) 
		return (0);

	signal(SIGINT, SIG_IGN); 
	signal(SIGQUIT, SIG_IGN); 

	if (mini->cmd_array[j].temp_heredoc == NULL) 
	{
		mini->cmd_array[j].temp_heredoc = ft_calloc(mini->cmd_array[j].compter_heredoc + 1, sizeof(char *)); 
		if (!mini->cmd_array[j].temp_heredoc)
		{
			mini->cmd_array[j].inout_fail = 1;
			mini->exit_status = 1;
			init_signaux(); 
			return (-1);
		}
	}
	while (n < mini->cmd_array[j].compter_heredoc) 
	{
		if (preparer_temp_file_name(mini, j, n) == -1)
		{
			mini->cmd_array[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
			mini->cmd_array[j].temp_heredoc = NULL;
			init_signaux(); 
			return (-1);
		}
		mini->cmd_array[j].pid_heredoc = fork(); 
		if (mini->cmd_array[j].pid_heredoc == -1) 
		{
			mini->cmd_array[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
			mini->cmd_array[j].temp_heredoc = NULL;
			init_signaux();
			return (-1); 
		}
		if (mini->cmd_array[j].pid_heredoc == 0) 
			appliquer_heredoc_enfant(mini, j, n);
		if (waitpid(mini->cmd_array[j].pid_heredoc, &status, 0) == -1) 
		{
			init_signaux();
			mini->cmd_array[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
			mini->cmd_array[j].temp_heredoc = NULL;
			return (-1); 
		}
		
		if (WIFSIGNALED(status))
		{
			exit_signal = WTERMSIG(status);
			mini->cmd_array[j].inout_fail = 1;
			mini->exit_status = 128 + exit_signal;
			free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
			mini->cmd_array[j].temp_heredoc = NULL;
			init_signaux();
			return (-1);
		}
		else if (WIFEXITED(status)) 
		{
			exit_status = WEXITSTATUS(status); 
			if (exit_status != 0) 
			{
				mini->cmd_array[j].inout_fail = 1; 
				mini->exit_status = exit_status; 
				free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
				mini->cmd_array[j].temp_heredoc = NULL;
				init_signaux(); 
				return (-1); 
			}
		}
		else 
		{
			mini->cmd_array[j].inout_fail = 1; 
			mini->exit_status = 1; 
			free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
			mini->cmd_array[j].temp_heredoc = NULL;
			init_signaux(); 
			return (-1);
		}
		if (!mini->cmd_array[j].temp_heredoc || !mini->cmd_array[j].temp_heredoc[n]) 
		{
			mini->cmd_array[j].inout_fail = 1;
			mini->exit_status = 1;
			free_temp_heredoc(mini->cmd_array[j].temp_heredoc);
			mini->cmd_array[j].temp_heredoc = NULL;
			init_signaux(); 
			return (-1);
		}
		n++;
	}
	init_signaux(); 
	return (0);
}

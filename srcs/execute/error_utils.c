#include "exec_run.h"

// int	msg(char *err)
// {
// 	if (!err)
// 		return (1);
// 	write(2, err, ft_strlen(err));
// 	if (err[ft_strlen(err) - 1] != '\n')
// 		write(2, "\n", 1);
// 	return (1);
// }

void	fatal_error(t_mini *mini, char *err)
{
	// 1. clean mini structure
	// 2. exit minishell

	// cleanup_all(mini);
	(void)mini;
	if (err)
		perror(err);
	else
		perror("Error");
	exit(EXIT_FAILURE);
}
// void	exec_exit(char *name, int exit_status)
// {
// 	if (name && *name)
// 		perror(name);
// 	else
// 		perror("minishell");
// 	exit (exit_status);
// }
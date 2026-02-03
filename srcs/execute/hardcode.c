#include <stdlib.h>
#include <string.h>
#include "exec_run.h"

t_mini *build_echo_cat_wc(void)
{
	t_mini *mini;

	mini = malloc(sizeof(t_mini));
	mini->env = NULL;
	mini->exit_status = 0;

	/* ---------- number of commands ---------- */
	mini->nbr_cmd = 3;
	mini->cmd_array = malloc(sizeof(t_cmd) * mini->nbr_cmd);

	/* ---------- cmd 0: echo hello ---------- */
	mini->cmd_array[0].cmd = malloc(sizeof(char *) * 3);
	mini->cmd_array[0].cmd[0] = strdup("echo");
	mini->cmd_array[0].cmd[1] = strdup("hello");
	mini->cmd_array[0].cmd[2] = NULL;

	mini->cmd_array[0].infile = NULL;
	mini->cmd_array[0].outfile = NULL;
	mini->cmd_array[0].out_append = 0;
	mini->cmd_array[0].heredoc = 0;
	mini->cmd_array[0].limiter = NULL;
	mini->cmd_array[0].in_fail = 0;
	mini->cmd_array[0].out_fail = 0;

	/* ---------- cmd 1: cat -e ---------- */
	mini->cmd_array[1].cmd = malloc(sizeof(char *) * 3);
	mini->cmd_array[1].cmd[0] = strdup("cat");
	mini->cmd_array[1].cmd[1] = strdup("-e");
	mini->cmd_array[1].cmd[2] = NULL;

	mini->cmd_array[1].infile = NULL;
	mini->cmd_array[1].outfile = NULL;
	mini->cmd_array[1].out_append = 0;
	mini->cmd_array[1].heredoc = 0;
	mini->cmd_array[1].limiter = NULL;
	mini->cmd_array[1].in_fail = 0;
	mini->cmd_array[1].out_fail = 0;

	/* ---------- cmd 2: wc -l ---------- */
	mini->cmd_array[2].cmd = malloc(sizeof(char *) * 3);
	mini->cmd_array[2].cmd[0] = strdup("wc");
	mini->cmd_array[2].cmd[1] = strdup("-l");
	mini->cmd_array[2].cmd[2] = NULL;

	mini->cmd_array[2].infile = NULL;
	mini->cmd_array[2].outfile = NULL;
	mini->cmd_array[2].out_append = 0;
	mini->cmd_array[2].heredoc = 0;
	mini->cmd_array[2].limiter = NULL;
	mini->cmd_array[2].in_fail = 0;
	mini->cmd_array[2].out_fail = 0;

	return mini;
}

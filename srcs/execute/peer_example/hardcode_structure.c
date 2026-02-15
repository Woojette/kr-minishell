#include <stdlib.h>
#include <string.h>
#include "model.h"

// echo hello | cat -e > out1 > out2

t_cmd_family *build_echo_cat_family(void)
{
	t_cmd_family *c1;
	t_cmd_family *c2;
	t_redirect   *r1;
	t_redirect   *r2;

	/* ---------- redirects: > out1 > out2 ---------- */
	r1 = malloc(sizeof(t_redirect));
	r1->type = REDIR_OUT;
	r1->filename = strdup("out1");

	r2 = malloc(sizeof(t_redirect));
	r2->type = REDIR_OUT;
	r2->filename = strdup("out2");

	r1->next = r2;
	r2->next = NULL;

	/* ---------- first cmd: echo hello ---------- */
	c1 = malloc(sizeof(t_cmd_family));
	c1->redirect = NULL;

	c1->cmd = malloc(sizeof(char *) * 3);
	c1->cmd[0] = strdup("echo");
	c1->cmd[1] = strdup("hello");
	c1->cmd[2] = NULL;

	/* ---------- second cmd: cat -e > out1 > out2 ---------- */
	c2 = malloc(sizeof(t_cmd_family));
	c2->redirect = r1;

	c2->cmd = malloc(sizeof(char *) * 3);
	c2->cmd[0] = strdup("cat");
	c2->cmd[1] = strdup("-e");
	c2->cmd[2] = NULL;

	/* ---------- link pipeline ---------- */
	c1->next = c2;
	c2->next = NULL;

	return c1;
}
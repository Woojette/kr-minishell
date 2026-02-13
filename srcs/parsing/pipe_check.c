#include "minishell.h"

// verifier s'il y a un pipe a la fin de la chaine ou l'espace seulement apres le dernier pipe
int	check_pipe_fin(char *line)
{
	int	i;

	i = 0;
	while (line[i]) // une boucle pour arriver a la fin de la chaine
		i++;
	// printf("index fin: %d\n", i);
	i--; // quand on sort la boucle, c'est '\0'. donc on avance une fois
	while (i >= 0 && line[i] == ' ') // avancer s'il y a l'espace a la fin
		i--;
	// printf("index avant pipe ou char: %d\n", i);
	if (i >= 0 && line[i] == '|') // verifier s'il y a un pipe apres l'espace
		return (1);
	return (0);
}

// compter le nombre de pipes dans la liste chainee
int count_pipe(t_token *token)
{
	int			count;
	t_token	*temp; // temporaire pour parcourir liste chainee token

	count = 0;
	temp = token;
	while (temp)
	{
		if (temp->type_token == T_PIPE)
			count++;
		temp = temp->next;
	}
	return (count);
}

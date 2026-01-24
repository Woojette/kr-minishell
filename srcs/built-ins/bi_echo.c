#include "minishell.h"

// Si on met -n, option_n = 1, 
// echo -nnn -n -nnn arg => option_n = 1 (affiche : arg)
// echo -nnnn -n -nnann -n arg => option_n = 1 (affiche : -nnann -n arg)
// sinon option_n = 0

int	ft_echo_option_n(char *str)
{
	int	i;

	if (!str)
		return (0);
	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] == 'n')
			i++;
		else
			return (0);
	}
	return (1);
}

void	ft_echo_all(char **tab)
{
	int	j;
	int	option_n;

	j = 1;
	option_n = 0;
	while (tab[j] != NULL && ft_echo_option_n(tab[j]) == 1)
		j++;
	if (j > 1)
		option_n = 1;
	while (tab[j] != NULL)
	{
		printf("%s", tab[j]);
		if (tab[j + 1] != NULL)
			printf(" ");
		j++;
	}
	if (option_n == 0)
		printf("\n");
}

// void	ft_echo(char *str)
// {
// 	int	option_n;

// 	option_n = ft_echo_option_n(str);
// 	printf("%s", str);
// 	if (option_n == 0)
// 		printf("\n");
// }

// void	ft_echo(char *str, int option_n)
// {
// 	printf("%s", str);
// 	if (option_n == 0)
// 		printf("\n");
// }

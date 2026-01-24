#include "minishell.h"

void	ft_unset_init_int_zero(int *j, int *supprime, int *taille)
{
	*j = 0;
	*supprime = 0;
	*taille = 0;
}

int	ft_unset(char *str, char ***env)
{
	int		j;
	int		supprime;
	int		taille;
	int		len_str;
	char	**env_supprime;

	ft_unset_init_int_zero(&j, &supprime, &taille);
	while ((*env)[taille] != NULL)
		taille++;
	len_str = ft_strlen(str);
	env_supprime = malloc(sizeof(char *) * (taille + 1));
	if (!env_supprime)
		return (-1);
	while ((*env)[j + supprime] != NULL)
	{
		if (((j + supprime) < taille) 
			&& ft_strncmp(str, (*env)[j + supprime], len_str) == 0
			&& (*env)[j + supprime][len_str] == '=')
			supprime++;
		if ((j + supprime) < taille)
		{
			env_supprime[j] = ft_strdup((*env)[j + supprime]);
			if (!env_supprime[j])
				return (ft_free_tab(env_supprime), -1);
		}
		j++; 
	}
	env_supprime[j] = NULL;
	ft_free_tab((*env));
	(*env) = env_supprime;
	return (0);
}

int	ft_unset_all(char **tab, char ***env)
{
	int	j;

	j = 1;
	while (tab[j] != NULL)
	{
		if (ft_unset(tab[j], env) == -1)
			return (-1);
		j++;
	}
	return (0);
}

#include "minishell.h"

int	ft_export_double(char *str, char ***env)
{
	int		j;
	int		taille;
	int		check;
	char	**env_ajoute;

	check = ft_check_env_double(str, (*env));
	j = 0;
	taille = 0;
	while ((*env)[taille] != NULL)
		taille++;
	env_ajoute = malloc(sizeof(char *) * (taille + 1));
	if (!env_ajoute)
		return (-1);
	while ((*env)[j] != NULL)
	{
		if (check == j)
			env_ajoute[j] = ft_strdup(str);
		else
			env_ajoute[j] = ft_strdup((*env)[j]);
		if (!env_ajoute[j])
			return (ft_free_tab(env_ajoute), -1);
		j++;
	}
	env_ajoute[j] = NULL;
	ft_free_tab((*env));
	(*env) = env_ajoute;
	return (0);
}

int	ft_export_sans_double(char *str, char ***env)
{
	int		j;
	int		taille;
	int		check;
	char	**env_ajoute;

	check = ft_check_env_double(str, (*env));
	j = 0;
	taille = 0;
	while ((*env)[taille] != NULL)
		taille++;
	env_ajoute = malloc(sizeof(char *) * (taille + 1 + 1));
	if (!env_ajoute)
		return (-1);
	while ((*env)[j] != NULL)
	{
		env_ajoute[j] = ft_strdup((*env)[j]);
		if (!env_ajoute[j])
			return (ft_free_tab(env_ajoute), -1);
		j++;
	}
	env_ajoute[j] = ft_strdup(str);
	if (!env_ajoute[j])
		return (ft_free_tab(env_ajoute), -1);
	j++;
	env_ajoute[j] = NULL;
	ft_free_tab((*env));
	(*env) = env_ajoute;
	return (0);
}

int	ft_export(char *str, char ***env)
{
	int		j;
	int		taille;
	int		check;

	j = 0;
	taille = 0;
	check = ft_check_env_double(str, (*env));
	if (check != 0)
	{
		if (ft_export_double(str, env) == -1)
			return (-1);
		return (0);
	}
	if (ft_export_sans_double(str, env) == -1)
		return (-1);
	return (0);
}

int	ft_export_all(char **tab, char ***env)
{
	int	j;

	j = 1;
	while (tab[j] != NULL)
	{
		if (ft_export(tab[j], env) == -1)
			return (-1);
		j++;
	}
	return (0);
}

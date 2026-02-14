/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_agrandi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// fonction pour agrandir un tableau et rajouter une chaine, comme pour le builtin export
// char **tab : double tableau actuel, *str : nouveau tableau a ajouter,
// int  size : taille actuelle de double tableau
char** add_double_tab_char(char **tab, char *str, int size)
{
	char **new_tab; // nouveau double tableau agrandi
	int j; // index pour parcourir les tableaux

	if ((!tab && size > 0) || !str || size < 0) // proteger au cas ou tab est NULL mais size > 0
		return (NULL);
	// if (!str || size < 0) // proteger au cas ou str est NULL ou size < 0
	// 	return (NULL);
	new_tab = malloc(sizeof(char *) * (size + 2)); // +1 pour le nouveau +1 pour NULL hihi
	if (!new_tab)
			return (NULL);
	j = 0;
	if (tab) // si tab n'est pas NULL, on copie les valeurs existantes
	{
		while (j < size)
		{
			new_tab[j] = tab[j]; // copier l'adresse de chaque chaine
			// cf) on copie juste le pointeur (l'adresse)
			// chaque tab[j] est un pointeur vers une chaine de caracteres
			// cf) char **tab = *tab[] = {"pho", "malatang", NULL}; (tableau de pinteurs vers des chaines)
			//     char tab[j] -> tab[0] = adresse de "pho", tab[1] = adresse de "malatang" (chaine de caracteres)
			j++;
		}
	}
	new_tab[j] = str; // l'adresse de nouvelle chaine ajoutee, on ajoute la nouvelle adresse ici youpiii
	new_tab[j + 1] = NULL; // terminer par NULL
	return (free(tab), new_tab); // vu qu'on a bien cree un nouveau tableau agrandi, on libere l'ancien tableau de pointeurs
}

// fonction pour agrandir un tableau et rajouter une valeur int (1 ou 0), comme pour out_append
int	*add_double_tab_int(int *tab, int val, int size)
{
	int *new_tab; // nouveau tableau agrandi
	int j; // index pour parcourir les tableaux

	if (!tab && size > 0) // proteger au cas ou tab est NULL mais size > 0
		return (NULL);
	if (size < 0)
		return (NULL);
	new_tab = malloc(sizeof(int) * (size + 1)); // +1 pour le nouveau 
	// cf) int *tab n'a pas besoin d'ajouter '\0' a la fin 
	if (!new_tab)
			return (NULL);
	j = 0;
	if (tab) // si tab n'est pas NULL, on copie les valeurs existantes
	{
		while (j < size)
		{
				new_tab[j] = tab[j]; // copier chaque valeur
				j++;
		}
	}
	new_tab[j] = val; // ajouter la nouvelle valeur (1 ou 0)
	free(tab); // vu qu'on a bien cree un nouveau tableau agrandi, on libere l'ancien tableau
	return (new_tab);
}

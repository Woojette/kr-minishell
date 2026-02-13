#include "minishell.h"

void inverser_et_incrementer(int *quote, int *i)
{
	*quote = !(*quote); // on inverse l'etat de quote (0 -> 1 , 1 -> 0)
	(*i)++; // on saute le caractere quote
}
// enlever les quotes dans un token str
char	*enlever_quote_dans_token(char *str)
{
	int	i; // index pour parcourir str
	int	n; // index pour parcourir resultat (nouveau str sans quotes)
	int	s_quote; // entree dans single quote ou pas (1 = dans single quote, 0 sinon)
	int	d_quote; // entree dans double quote ou pas (1 = dans double quote, 0 sinon)
	char	*resultat; // le nouveau str sans quotes

	i = 0;
	n = 0;
	s_quote = 0; // pour gerer le cas de dans single quote (1 = dans single quote, 0 sinon)
	d_quote = 0; // pour gerer le cas de double quote (1 = dans double quote, 0 sinon)
	if (!str)
		return (NULL);
	resultat = malloc(sizeof(char) * (ft_strlen(str) + 1)); // resultat ne peut pas etre plus long que str (puisqu'on va juste enlever les quotes)
	// donc on alloue la taille de str + 1 pour le '\0'
	if (!resultat)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote) // gerer l'ouverture/fermeture de single quote  (on ne traite pas les quotes dans les double quotes)
			inverser_et_incrementer(&s_quote, &i);

		else if (str[i] == '"' && !s_quote) // gerer le cas de double quote au debut
			inverser_et_incrementer(&d_quote, &i);
		else
			resultat[n++] = str[i++]; // on copie le caractere dans resultat et on avance les index
		// c'est pareil que resulat[n] = str[i]; n++; i++; hihi j'ai appris
		// ex) you"pi'i'i" -> youpii
	}
	resultat[n] = '\0'; // terminer resultat par '\0'
	if (s_quote || d_quote) // si on a une quote non fermee, on retourne NULL (erreur de syntaxe)
		return (free(resultat), NULL);
	return (resultat); // retourner le nouveau str sans quotes
}

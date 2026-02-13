#include "minishell.h"

void inverser_et_incrementer(int *quote, int *i)
{
	*quote = !(*quote); // on inverse l'etat de quote (0 -> 1 , 1 -> 0)
	(*i)++; // on saute le caractere quote
}

void	init_enlever_quote(t_quote *quote)
{
	quote->i = 0;
	quote->n = 0;
	quote->s_quote = 0; // pour gerer le cas de dans single quote (1 = dans single quote, 0 sinon)
	quote->d_quote = 0; // pour gerer le cas de double quote (1 = dans double quote, 0 sinon)
}

void	quote_a_enlever(char *str, char *resultat, t_quote *quote)
{
	if (str[quote->i] == '\'' && !quote->d_quote) // gerer l'ouverture/fermeture de single quote  (on ne traite pas les quotes dans les double quotes)
		inverser_et_incrementer(&quote->s_quote, &quote->i);
	else if (str[quote->i] == '"' && !quote->s_quote) // gerer le cas de double quote au debut
		inverser_et_incrementer(&quote->d_quote, &quote->i);
	else
		resultat[quote->n++] = str[quote->i++]; // on copie le caractere dans resultat et on avance les index
	// c'est pareil que resulat[n] = str[i]; n++; i++; hihi j'ai appris
	// ex) you"pi'i'i" -> youpii
}

// enlever les quotes dans un token str
char	*enlever_quote_dans_token(char *str)
{
	t_quote	quote; // structure pour gerer les quotes
	char	*resultat; // le nouveau str sans quotes

	if (!str)
		return (NULL);
	init_enlever_quote(&quote);
	resultat = malloc(sizeof(char) * (ft_strlen(str) + 1)); // resultat ne peut pas etre plus long que str (puisqu'on va juste enlever les quotes)
	// donc on alloue la taille de str + 1 pour le '\0'
	if (!resultat)
		return (NULL);
	while (str[quote.i])
		quote_a_enlever(str, resultat, &quote);
	resultat[quote.n] = '\0'; // terminer resultat par '\0'
	if (quote.s_quote || quote.d_quote) // si on a une quote non fermee, on retourne NULL (erreur de syntaxe)
		return (free(resultat), NULL);
	return (resultat); // retourner le nouveau str sans quotes
}

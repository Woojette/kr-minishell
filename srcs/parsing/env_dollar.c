#include "minishell.h"


//quote -> 1 = simple quote, 2 = double quote, 0 = pas de quote
// initialiser les variables pour remplacer_dollar
void  init_val(int *i, int *s_quote, int *d_quote, char **resultat)
{
  *i = 0; // index pour parcourir resultat
  *s_quote = 0; // pour gerer le cas de single quote (0 = pas dans single quote, 1 = dans single quote)
  *d_quote = 0; // pour gerer le cas de double quote (0 = pas dans double quote, 1 = dans double quote)
  (*resultat) = malloc(sizeof(char) * 1); // on initialise resultat avec 1 caractere (pour resultat[0]='\0')
  if (!(*resultat))
    return ;
  (*resultat)[0] = '\0'; // str est vide au depart
}

char *ajouter_char_et_inverser_quote(char *resultat, int *i, int *quote, char *str)
{
  (*quote) = !(*quote); // on inverse l'etat de quote (0 -> 1 , 1 -> 0)
  resultat = ajouter_char(resultat, str[(*i)++]); // on ajoute le caractere quote a resultat
  return (resultat);
}

// remplacement de $ par la valeur de la variable d'env
// parcours le token str et creer un nouveau str* result qui remplacera str;
// Si c’est un caractere $, on cherche la variable d’environment et on remplace
// sinon juste on copie caractere par caractere result est vide au depart et on le construit caractre par caractere
// le cas de single quote a gerer aussi (ne pas remplacer $ dans des single quote) *******************************
char	*remplacer_dollar(char *str, t_mini *mini)
{
	int		i; // l'index pour parcourir *str
	int		s_quote;
	int		d_quote;
	char	*resultat; // le nouveau str qui remplace *str

	if (!str || !mini)
		return (NULL);
  init_val(&i, &s_quote, &d_quote, &resultat);
	if (!resultat)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote) // gerer le cas de single quote au debut
      resultat = ajouter_char_et_inverser_quote(resultat, &i, &s_quote, str);
		else if (str[i] == '"' && !s_quote) // gerer le cas de double quote au debut
      resultat = ajouter_char_et_inverser_quote(resultat, &i, &d_quote, str);
		// si on voit $ (et pas $ a la fin de chaine)
		// on remplace par la valeur du nom et uniquement si on est pas dans des single quote
		else if ((str[i] == '$' && str[i + 1] != '\0') && !s_quote)
			resultat = appliquer_env_var(resultat, str, mini, &i); // on passe l'adresse de i pour le modifier dans la fonction
		else // sinon on copie caractere par caractere str vers resultat
			resultat = ajouter_char(resultat, str[i++]);
		if (!resultat)
			return (NULL);
	}
	return (resultat);
}

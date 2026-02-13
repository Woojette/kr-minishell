#include "minishell.h"

// Verifie les quotes, dès qu’on croise une quote, on cherche sa paire
// si on arrive a la fin avant = unclose quote
// a verifier avant de commencer le parsing
int	check_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	if (!line[i]) // si la chaine est vide (line[0] = '\0')
		return (0);
	while (line[i])
	{
		// a partir de la, on cherche les quotes
		if (line[i] == '"' || line[i] == '\'') // si on croise une quote, on entre dans la boucle pour trouver une autre
		{
			quote = line[i]; // sauvegarder la premiere quote
			i++; // puis, commencer a chercher la deuxieme
			while (line[i] && line[i] != quote) // ce qui n'est pas une quote pareille que la premiere, on passe
				i++;
			// on sort de la boucle soit quand on trouve la quote pareille (bien fermee), soit a la fin de la chaine
			if (!line[i]) // on est a la fin de la chaine ('\0'), sans trouver la quote fermante -> 0
				return (0);
		}
		i++; 
		// soit on a trouve la quote fermante, soit on n'a pas encore trouve de quote, 
		// on continue a parcourir le caractere suivant de la chaine
		// ca permet de verifier toutes les quotes dans la chaine (ex. echo "hihi"coucou'you'pi)
	}
	return (1); // si les quotes dans la chaine sont appariees et fermees -> 1
}

// verifier s'il y a un espace (redir, pipe) avant la premiere quote
int check_avant_quote_espace(char *line)
{
    int i;
    int pos_quote;

    if (!line || !*line)
        return 0;
    // cherecher la premiere quote
    i = 0;
    while (line[i] && line[i] != '"' && line[i] != '\'')
        i++;
    if (!line[i])
        return (0);
    pos_quote = i;  // l'index de la premiere quote
    if (pos_quote == 0) // s'il y a pas de quote dans la chaine, pas besoin de chercher
        return (1);
    i = pos_quote - 1; // avant la quote
    while (i >= 0) // on cherche un espace (redir, pipe) jusqu'a tout au debut de la chaine
    {
        if (line[i] == ' ' || line[i] == '>' || line[i] == '<' || line[i] == '|')
            return 1;
        i--;
    }
    return (0);
}

// fonction qui verifie l'espace (ou '\0', redir, pipe) apres la 2e quote
// au cas ou le premier caractere ne commence pas par une quote
int check_2_quotes_milieu_puis_fin(char *line)
{
	int		i;
	char	debut_quote; // pour sauvegarder la premiere quote

	debut_quote = caractere_quote_debut(line);
	if (debut_quote == 0 || !check_quote_milieu_ok(line)) // s'il n'y a pas de quote dans la chaine, pas besoin de chercher
		return (-1);
	i = index_quote_debut(line, debut_quote);
	if (i < 0)
		return (-1);
	i++;
	while (line[i])
	{
		if (line[i] == debut_quote)
		{
			i++;
			if (line[i] == ' ' || line[i] == '\0'
				|| line[i] == '>' || line[i] == '<' || line[i] == '|') // si on a l'espace (ou '\0', redir, pipe) apres 2e quote -> 1
				return (1); // ex) echo hi"hi"  coucou, echo hi'hi' | cat -e
			return (0); // quotes puis caractere -> 0  ex) echo you"p"i, echo you'p'i
		}
		i++;
	}
	return (0); // quotes puis caractere -> 0  ex) echo you"p"i
}

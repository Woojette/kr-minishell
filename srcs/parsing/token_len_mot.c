#include "minishell.h"

// compter len du type mot (avec quote + sans quote)
int	len_mot_total(char *line)
{
	int	len;

	len = 0;
	// 1. le cas ou le premier caractere est une quote  ex) "hihi", "hihi, 'hihi', 'hihi, 'hi"hi, etc
	if (line[0] == '"' || line[0] == '\'')
	{
		// 1-1.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) fin (' ' ou '\\0' ou redir ou pipe) apres la 2e quote
		if (check_quote_debut_ok(line) == 1 && check_2_quotes_debut_puis_fin(line) == 1) // ex) echo "hihi" coucou,  echo "hihi", echo "hihi"| ~~~
			len = len_mot_2_quotes_entier(line); // calcule a partir de la 1e quote a la 2e quote  ex) "..."
		// 1-2.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) un caractere apres la 2e quote
		else if (check_quote_debut_ok(line) == 1 && check_2_quotes_debut_puis_fin(line) == 0)
			len = len_mot_2_quotes_entier(line) + len_mot_apres_quote(line); // ex) 'you'pi ->  strlen("'you'") + strlen("pi")
		// 1-3.  1) le premier caractere = quote   2) 2 quotes pas fermees
		else if (check_quote_debut_ok(line) == 0) // ex) ', ", "', '", "'', '"", "'''', '''", echo "hi, echo 'hi |
			len = len_mot_sans_quote(line);
	}
	// 2. le cas ou le premier caractere ne commence pas par une quote (mais pas redir, ni pipe non plus)
	else if (line[0] != '"' && line[0] != '\'')
	{
		// 2-1.  1) quote au milieu   2) 2 quotes bien fermees   3) apres quote ' ' ou redir ou pipe
		if (check_quote_milieu_ok(line) == 1 && check_avant_quote_espace(line) == 0 && check_2_quotes_milieu_puis_fin(line) == 1)
		{
			// printf("test  2-1a\\n");
			len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line);
		}
		// 2-2.  1) quote au milieu   2) 2 quotes bien fermees   3) caractere apres la 2e quote
		else if (check_quote_milieu_ok(line) == 1 && check_avant_quote_espace(line) == 0 &&  check_2_quotes_milieu_puis_fin(line) == 0)
		{
			// printf("test  2-2\\n");
			len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line) + len_mot_apres_quote(line);
		}
		// 2-3.  1) pas de quote
		else if (check_quote_milieu_ok(line) == 0 && check_avant_quote_espace(line) == 0)
		{
			// printf("test 2-3\\n");
			len = len_mot_sans_quote(line);
		}
		// 2-4. proteger au cas ou on sait jamais
		else
		{
			// printf("test 2-4\\n");
			len = len_mot_sans_quote(line);
		}
	}
	return (len);
}

// recuperer len avant la quote qui est au milieu de la chaine
int	len_mot_avant_quote(char *line)
{
	int		i;
	int		quote;
	char	debut_quote;

	i = 0;
	debut_quote = caractere_quote_debut(line);
	quote = index_quote_debut(line, debut_quote);
	while (i < quote)
		i++;
	return (i); // ex) you"pi" (-> 3)
}

// compter le nombre de caracteres entre 2 quotes, y compris les 2 quotes  ex) "...", '...' 
int	len_mot_2_quotes_entier(char *line)
{
	char	debut_quote; // recuperer la quote de premier caractere de la chaine
	int		i;
	int		len;

	debut_quote = caractere_quote_debut(line);
	i = index_quote_debut(line, debut_quote) + 1;
	len = 1;
	while (line[i])
	{
		if (line[i] == debut_quote)
		{
			len++;
			break ;
		}
		len++;
		i++;
	}
	return (len);
}

// recuperer len apres la 2e quote
int	len_mot_apres_quote(char *line)
{
	int		i;
	int		len_apres_quote;
	char	quote;

	quote = caractere_quote_debut(line);
	i = index_quote_fin(line, quote) + 1;
	len_apres_quote = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\0'
			|| line[i] == '>' || line[i] == '<' || line[i] == '|')
			break;
		len_apres_quote++;
		i++;
	}
	return (len_apres_quote);
}

// compter le nombre de chaines dans un double tableau char**
int	len_tab_char(char **tab)
{
	int	j;

	j = 0;
	if (!tab)
		return (0);
	while (tab[j])
		j++;
	return (j);
}

// compter le nombre de caracteres s'il y a pas de 2 quotes qui fonctionnent
int	len_mot_sans_quote(char *line)
{
	int	i;

	i = 0;
	while ((*line) && (*line) != ' ' && (*line) != '\t'
		&& (*line) != '>' && (*line) != '<' && (*line) != '|'
		&& (*line) != '"' && (*line) != '\'')
	{
		line++;
		i++;
	}
	return (i);
}

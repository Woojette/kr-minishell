#include "minishell.h"

int	appliquer_quote_premier(char *line, int *len)
{
	int	debut_quote; // pour recuperer le type de quote du premier caractere de la chaine
	int	quote_fermee; // pour verifier si la quote est fermee ou pas

	debut_quote = check_quote_debut_ok(line);
	quote_fermee = check_2_quotes_debut_puis_fin(line);
	// 1-1.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) fin (' ' ou '\\0' ou redir ou pipe) apres la 2e quote
	if (debut_quote == 1 && quote_fermee == 1) // ex) echo "hihi" coucou,  echo "hihi", echo "hihi"| ~~~
		*len = len_mot_2_quotes_entier(line); // calcule a partir de la 1e quote a la 2e quote  ex) "..."
	// 1-2.  1) le premier caractere = quote   2) 2 quotes bien fermees   3) un caractere apres la 2e quote
	else if (debut_quote == 1 && quote_fermee == 0)
		*len = len_mot_2_quotes_entier(line) + len_mot_apres_quote(line); // ex) 'you'pi ->  strlen("'you'") + strlen("pi")
	// 1-3.  1) le premier caractere = quote   2) 2 quotes pas fermees
	else if (debut_quote == 0) // ex) ', ", "', '", "'', '"", "'''', '''", echo "hi, echo 'hi |
		*len = len_mot_sans_quote(line);
	else
		return (0);
	return (1);
}

int	appliquer_quote_milieu(char *line, int *len)
{
	int	milieu_quote; // pour verifier s'il y a une quote au milieu de la chaine
	int	espace_avant_quote; // pour verifier s'il y a un espace (redir, pipe) avant la quote au milieu de la chaine
	int	quote_fermee; // pour verifier si les 2 quotes au milieu de la chaine sont fermees ou pas

	milieu_quote = check_quote_milieu_ok(line);
	espace_avant_quote = check_avant_quote_espace(line);
	quote_fermee = check_2_quotes_milieu_puis_fin(line);
	// 2-1.  1) quote au milieu   2) 2 quotes bien fermees   3) apres quote ' ' ou redir ou pipe
	if (milieu_quote == 1 && espace_avant_quote == 0 && quote_fermee == 1)
		*len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line);
	// 2-2.  1) quote au milieu   2) 2 quotes bien fermees   3) caractere apres la 2e quote
	else if (milieu_quote == 1 && espace_avant_quote == 0 &&  quote_fermee == 0)
		*len = len_mot_avant_quote(line) + len_mot_2_quotes_entier(line) + len_mot_apres_quote(line);
	// 2-3.  1) pas de quote
	else if (milieu_quote == 0 && espace_avant_quote == 0)
		*len = len_mot_sans_quote(line);
	// 2-4. proteger au cas ou on sait jamais
	else
		*len = len_mot_sans_quote(line);
	return (1);
}

// compter len du type mot (avec quote + sans quote)
int	len_mot_total(char *line)
{
	int	len;

	len = 0;
	// 1. le cas ou le premier caractere est une quote  ex) "hihi", "hihi, 'hihi', 'hihi, 'hi"hi, etc
	if (!line || line[0] == ' ' || line[0] == '\t' || line[0] == '>' || line[0] == '<' || line[0] == '|')
		return (0);
	if (line[0] == '"' || line[0] == '\'')
	{
		if (appliquer_quote_premier(line, &len) == 1)
			return (len);
		return (len_mot_sans_quote(line));
	}
	// 2. le cas ou le premier caractere ne commence pas par une quote (mais pas redir, ni pipe non plus)
	else if (line[0] != '"' && line[0] != '\'')
	{
		if (appliquer_quote_milieu(line, &len) == 1)
			return (len);
	}
	return (len);
}

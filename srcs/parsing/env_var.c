#include "minishell.h"

// recuperer le nom de la variable d'env apres $
// a partir de l'index start (apres $) de str, on va recuperer le nom de la variable d'env
// ex) $USER -> start = index de U
char	*get_env_name(char *str, int start)
{
	int	i; // index pour parcourir str
	int	len; // la taille du nom de la variable d'env

	len = 0;
	i = start; // on va compter la taille a partir de l'index start
	if (!str || start < 0 || !str[start]) // proteger au cas ou str est NULL ou start est hors de la chaine
		return (NULL);
	if (str[i] == '?') // cas special pour $?
		return (ft_substr(str, start, 1));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_')) 
	// le nom de variable peut etre soit l'alphabet soit le chiffre soit '_' (dans ce cas on continue pour compter la taille)
	{
		i++;
		len++;
	}
	if (len == 0) // s'il y a pas de variable apres $ , on return NULL
		return (NULL);
	return (ft_substr(str, start, len));
	// on retourne le nom de la variable d'env 
	// qui est a partir de l'index start et de taille len
}

// recuperer $ env variable
char	*get_env_var(char *str, t_mini *mini)
{
	int	j;
	int	len;

	j = 0;
	if (!str || !mini || !mini->env)
		return (NULL);
	if (str[0] == '?' && str[1] == '\0') // cas special pour $? (convertir le code de sortie en chaîne)
		return (ft_itoa(mini->exit_status)); // pour convertir exit status en str
		// exit status est un int, donc on utilise ft_itoa pour le convertir en str, pour afficher dans notre minishell
		// vu qu'on a alloue le memoire dans ft_itoa, il faut liberer apres l'utilisation ************
	len = ft_strlen(str);
	while ((mini->env)[j]) // on va parcourir tout env
	{
		if (ft_strncmp((mini->env)[j], str, len) == 0 && (mini->env)[j][len] == '=')
			return (ft_strdup((mini->env)[j] + (len + 1))); // on retourne le contenu apres le '=' (free apres *****)
		j++;
	}
	return (ft_strdup("")); // si on ne trouve pas la variable d'env, on retourne une chaine vide
}

// ajouter un char c a la fin de la chaine resultat  
// ex) resultat = "youp", c = 'i' -> return "youpi"
char	*ajouter_char(char *resultat, char c)
{
	int		len; // la taille de resultat
	char	*temp; // temporaire pour stocker le nouveau resultat

	len = ft_strlen(resultat);
	temp = malloc(sizeof(char) * (len + 2)); // on agrandit de 2 pour le char c et le '\0'
	if (!temp)
		return (free(resultat), NULL);
	ft_strcpy(temp, resultat); // on copie resultat dans temp
	temp[len] = c; // on rajoute le char c passsé en parametre
	temp[len + 1] = '\0'; // et on termine avec 0
	free(resultat); // on libere l'ancien resultat
	return (temp);
}

// appliquer la variable d'env dans str a la position i (qui est le $)
// str = le str entier du token
// resultat = le nouveau str qui va etre créé
// i = la position du $ dans str (pour changer le variable d'origine, on utilise son pointeur)
// mini = la structure principale qui contient env et exit status
char	*appliquer_env_var(char *resultat, char *str, t_mini *mini, int *i)
{
	char	*env_name; // le nom de la variable d'env apres $ (ex. USER de $USER)
	char	*env_var; // la valeur de la variable d'env
	char	*temp; // temporaire pour stocker le nouveau resultat

	env_name = get_env_name(str, *i + 1); // on recupere le nom de la variable d'env apres $  (cf. *i + 1 <- la position apres $)
	// apres $, s'il y a un nom alphabetique ou chiffre ou '_', on le recupere
	// cf) il est possible de nom recupere ne soit pas dans env (ex. $qui_nexiste_pas)
	if (env_name) // si on a un nom de variable d'env
	{	
		env_var = get_env_var(env_name, mini); // on recupere la valeur de la variable d'env
		if (!env_var)
			return (free(env_name), free(resultat), NULL);
		temp = ft_strjoin(resultat, env_var); // on concatene le resultat avec la valeur de la variable d'env
		if (!temp)
			return (free(env_var), free(env_name), free(resultat), NULL);
		free(env_var); // on libere env_var, puisqu'on l'a deja utilise
		free(resultat); // on libere l'ancien resultat
		resultat = temp; // on met a jour resultat
		*i = *i + ft_strlen(env_name) + 1; // +1 pour $
		free(env_name); // on libere env_name, puisqu'on l'a deja utilise
		return (resultat); // on retourne le resultat mis a jour
	}
	else // si on n'a pas de nom de variable d'env apres $  (ex. env_name == NULL ,  echo $)
		return (ajouter_char(resultat, str[(*i)++])); // on ajoute juste le caractere $ a resultat
		// (*i) : vu que i est le pointeur de int, pour recuperer la valeur, on fait *i
		// on applique la valeur str[(*i)] a la fonction ajouter_char
		// (*i)++ : puis, incrementer de 1 a la valeur (*i)  
}
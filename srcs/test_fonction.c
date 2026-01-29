git// #include "test.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	char	*resultat;
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(str);
	resultat = malloc(sizeof(char) * (len + 1));
	if (resultat == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		resultat[i] = str[i];
		i++;
	}
	resultat[i] = '\0';
	return (resultat);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		resultat;

	i = 0;
	resultat = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && i < n)
	{
		if (s1[i] != s2[i])
		{
			resultat = (unsigned char)s1[i] - (unsigned char)s2[i];
			return (resultat);
		}
		i++;
	}
	return (0);
}

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c = c - 32;
	return (c);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	size_t			n;
	char			*resultat;

	i = 0;
	n = 0;
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	resultat = malloc((len + 1) * sizeof(char));
	if (resultat == NULL)
		return (NULL);
	while (s[start + i] != '\0' && n < len && start < ft_strlen(s))
	{
		resultat[i] = s[start + i];
		n++;
		i++;
	}
	resultat[i] = '\0';
	return (resultat);
}

void	*ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i++;
	}
	return (s);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*resultat;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2);
	resultat = malloc(sizeof(char) * (len + 1));
	if (resultat == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		resultat[i + j] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		resultat[i + j] = s2[j];
		j++;
	}
	resultat[i + j] = '\0';
	return (resultat);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (0);
	return (1);
}

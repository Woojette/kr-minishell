/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:02:07 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 16:15:51 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*resultat;

	if (!s1 || !s2)
		return (NULL);
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
/*
int	main(void)
{
	char	texte1[] = "j'aime ";
	char	texte2[] = "le moineau !";
	char	*resultat = ft_strjoin(texte1, texte2);
	printf("%s\n", resultat);
	free(resultat);
	return (0);
}*/

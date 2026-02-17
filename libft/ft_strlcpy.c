/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:54:23 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:59:20 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	if (size == 0)
		return (ft_strlen(src));
	while ((src[i]) && (i < size - 1))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	while (src[len] != '\0')
		len++;
	return (len);
}
/*
int	main(void)
{
	char	dest00[] = "youpi";
	char	dest01[] = "youpi";
	char	dest10[] = "coucou";
	char	dest11[] = "coucou";
	char	dest30[] = "aujourd'hui je travaille pas youpii";
	char	dest31[] = "aujourd'hui je travaille pas youpii";
	char	faim[] = "j'ai faim.";

	printf("strlen faim = %zu\n\n", strlen(faim));
	printf("youpi / 3 = %zu\n", ft_strlcpy(dest00, faim, 3));
	printf("youpi / 3 = %zu vrai\n", strlcpy(dest01, faim, 3));
	printf("dest = %s\n", dest00);
	printf("dest = %s vrai\n\n", dest01);
	printf("coucou / 2 = %zu\n", ft_strlcpy(dest10, faim, 2));
	printf("coucou / 2 = %zu vrai\n", strlcpy(dest11, faim, 2));
	printf("dest = %s\n", dest10);
	printf("dest = %s vrai\n\n", dest11);
	printf("long texte / 100 = %zu\n", strlcpy(dest30, faim, 100));
	printf("long texte / 100 = %zu vrai\n", strlcpy(dest31, faim, 100));
	printf("dest = %s\n", dest30);
	printf("dest = %s\n\n", dest31);
	return (0);
}*/

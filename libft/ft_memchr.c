/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:31:41 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 16:11:26 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			return ((void *)(&s)[i]);
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	char	texte[] = "je vais boire ce soir.";
	char	texte_vrai[] = "je vais boire ce soir.";
	printf("b = %p (ft)\n", ft_memchr(texte, 'b', 15));
	printf("b = %p (vrai)\n", memchr(texte_vrai, 'b', 15));
	printf("ab = %p (ft)\n", ft_memchr(texte, '0', 15));
	printf("ab = %p (vrai)\n", memchr(texte_vrai, '0', 15));
	return (0);
}*/

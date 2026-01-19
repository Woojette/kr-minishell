/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:40:22 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/01 17:46:56 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    *ft_strndup(const char *s, size_t n)
{
    size_t    i;
    size_t    len;
    char       *dup;

    i = 0;
    len = ft_strlen(s);
    if (len > n)
        len = n;
    dup = (char *)malloc(sizeof(char) * (len + 1));
    if (dup == NULL)
        return (NULL);
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}
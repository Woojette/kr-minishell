/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/16 21:20:01 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	buf[1024];
	size_t	size;

	size = sizeof(buf);
	if (getcwd(buf, size) != NULL)
	{
		printf("%s\n", buf);
		return (0);
	}
	else
	{
		printf("pwd: error retrieving current directory: ");
		printf("getcwd: cannot access parent directories");
		printf(": No such file or directory");
		printf("\n");
	}
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dups.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:42:21 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/15 17:16:06 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	p_dup2(t_mini *mini, int fd_has, int fd_to)
{
	int	result;

	result = dup2(fd_has, fd_to);
	if (result == -1)
		p_exit(mini);
}

void	c_dup2(t_mini *mini, int fd_has, int fd_to)
{
	int	result;

	result = dup2(fd_has, fd_to);
	if (result == -1)
		child_exit(mini);
}

void	ft_close(int fd)
{
	int result;

	result = close(fd);
	if (result == -1)
	{
		perror("close problem");
	}
}

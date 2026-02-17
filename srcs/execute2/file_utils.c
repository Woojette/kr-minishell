/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:16:32 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 01:29:20 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(char *file_path)
{
	struct stat	buf;

	if (stat(file_path, &buf) == -1)
		return (0);
	return (S_ISDIR(buf.st_mode));
}

int	is_executable(char *file_path)
{
	struct stat	buf;

	if (stat(file_path, &buf) == -1)
	{
		return (0);
	}
	return (__S_IEXEC & buf.st_mode);
}

int	does_file_exist(char *file_path)
{
	struct stat	buf;

	if (stat(file_path, &buf) == 0)
		return (1);
	return (0);
}

int	cmd_qqpart(t_mini *mini)
{
	int	i;

	i = 0;
	if (!mini || !mini->cmd_array)
		return (0);
	while (i < mini->nbr_cmd)
	{
		if (mini->cmd_array[i].cmd && mini->cmd_array[i].cmd[0])
			return (1);
		i++;
	}
	return (0);
}

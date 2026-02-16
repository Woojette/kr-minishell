/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:17:38 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/16 04:53:16 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_path(t_mini *mini)
{
	int	i;

	i = 0;
	if (!mini->env)
		return (NULL);
	while (mini->env[i])
	{
		if (ft_strncmp(mini->env[i], "PATH=", 5) == 0)
			return (mini->env[i] + 5);
		i++;
	}
	return (NULL);
}

void	set_path_array(t_mini *mini)
{
	char	*env_path;
	int		i;
	char	*tmp;

	if (mini->path_array)
		return ;
	env_path = get_env_path(mini);
	if (!env_path)
		fatal_error(mini, "PATH not found");
	mini->path_array = ft_split(env_path, ':');
	if (!mini->path_array)
		fatal_error(mini, "malloc error on PATH split");
	i = 0;
	while (mini->path_array[i])
	{
		tmp = mini->path_array[i];
		mini->path_array[i] = ft_strjoin(mini->path_array[i], "/");
		free(tmp);
		i++;
	}
}

char	*get_path_absolute(t_mini *mini, char *cmd)
{
	if (does_file_exist(cmd))
	{
		if (is_directory(cmd))
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Is a directory\n", 17);
			mini->exit_status = 126;
			return (NULL);
		}
		else if (!is_executable(cmd))
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": Permission denied\n", 20);
			mini->exit_status = 126;
			return (NULL);
		}
		else
			return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*get_path_envp(t_mini *mini, char *cmd)
{
	int		i;
	char	*whole;

	i = 0;
	while (mini->path_array[i])
	{
		whole = ft_strjoin(mini->path_array[i], cmd);
		if (!whole)
			return (NULL);
		if (does_file_exist(whole))
		{
			if (is_directory(whole) || !is_executable(whole))
			{
				mini->exit_status = 126;
				return (free(whole), NULL);
			}
			else
				return (whole);
		}
		free(whole);
		i++;
	}
	return (mini->exit_status = 127, NULL);
}

char	*cmd_path_center(t_mini *mini, char *cmd)
{
	char	*found_path;

	if (!mini || !cmd || !cmd[0])
	{
		mini->exit_status = 0;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
	{
		found_path = get_path_absolute(mini, cmd);
		if (!found_path && mini->exit_status == 0)
			mini->exit_status = 127;
		return (found_path);
	}
	// found_path = get_path_absolute(mini, cmd);
	// if (found_path)
	// 	return (found_path);
	// if (mini->exit_status == 126)
	// 	return (NULL);
	set_path_array(mini);
	found_path = get_path_envp(mini, cmd);
	// if (!found_path && mini->exit_status == 0)
	// 	mini->exit_status = 127;
	return (found_path);
}

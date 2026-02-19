/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/16 21:51:54 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_cd_val_env(char *str, t_mini *mini)
{
	int		j;
	char	*str_path;

	j = 0;
	while ((mini->env)[j] != NULL)
	{
		if (ft_strncmp((mini->env)[j], str, ft_strlen(str)) == 0)
		{
			str_path = ft_strdup((mini->env)[j] + ft_strlen(str));
			if (!str_path)
				return (NULL);
			return (str_path);
		}
		j++;
	}
	return (NULL);
}

int	ft_cd_env_update(char *oldpwd, char *pwd, t_mini *mini)
{
	int		j;
	char	*temp;

	j = 0;
	while ((mini->env)[j] != NULL)
	{
		if (ft_strncmp((mini->env)[j], "OLDPWD=", 7) == 0 && oldpwd)
		{
			temp = ft_strjoin("OLDPWD=", oldpwd);
			if (!temp)
				return (free(pwd), -1);
			free((mini->env)[j]);
			(mini->env)[j] = temp;
		}
		else if (ft_strncmp((mini->env)[j], "PWD=", 4) == 0 && pwd)
		{
			temp = ft_strjoin("PWD=", pwd);
			if (!temp)
				return (free(oldpwd), -1);
			free((mini->env)[j]);
			(mini->env)[j] = temp;
		}
		j++;
	}
	return (0);
}

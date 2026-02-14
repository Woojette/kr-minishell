/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_name(char *str, int start)
{
	int	i;
	int	len;

	len = 0;
	i = start;
	if (!str || start < 0 || !str[start])
		return (NULL);
	if (str[i] == '?')
		return (ft_substr(str, start, 1));
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		i++;
		len++;
	}
	if (len == 0)
		return (NULL);
	return (ft_substr(str, start, len));
}

char	*get_env_var(char *str, t_mini *mini)
{
	int	j;
	int	len;

	j = 0;
	if (!str || !mini || !mini->env)
		return (NULL);
	if (str[0] == '?' && str[1] == '\0')
		return (ft_itoa(mini->exit_status));
	len = ft_strlen(str);
	while ((mini->env)[j])
	{
		if (ft_strncmp((mini->env)[j], str, len) == 0
			&& (mini->env)[j][len] == '=')
			return (ft_strdup((mini->env)[j] + (len + 1)));
		j++;
	}
	return (ft_strdup(""));
}

char	*ajouter_char(char *resultat, char c)
{
	int		len;
	char	*temp;

	len = ft_strlen(resultat);
	temp = malloc(sizeof(char) * (len + 2));
	if (!temp)
		return (free(resultat), NULL);
	ft_strcpy(temp, resultat);
	temp[len] = c;
	temp[len + 1] = '\0';
	free(resultat);
	return (temp);
}

char	*appliquer_env_var(char *resultat, char *str, t_mini *mini, int *i)
{
	char	*env_name;
	char	*env_var;
	char	*temp;

	env_name = get_env_name(str, *i + 1);
	if (env_name)
	{
		env_var = get_env_var(env_name, mini);
		if (!env_var)
			return (free(env_name), free(resultat), NULL);
		temp = ft_strjoin(resultat, env_var);
		if (!temp)
			return (free(env_var), free(env_name), free(resultat), NULL);
		free(env_var);
		free(resultat);
		resultat = temp;
		*i = *i + ft_strlen(env_name) + 1;
		free(env_name);
		return (resultat);
	}
	else
		return (ajouter_char(resultat, str[(*i)++]));
}

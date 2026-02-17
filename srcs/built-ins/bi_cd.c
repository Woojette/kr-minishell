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

int	ft_cd_sans_av(char **val, char **path, char *str, t_mini *mini)
{
	char	*new_oldpwd;
	char	*new_pwd;

	(*val) = ft_cd_val_env(str, mini);
	if (!(*val))
	{
		if (ft_strncmp(str, "HOME=", 5) == 0)
			printf("minishell: cd: HOME not set\n");
		else if (ft_strncmp(str, "OLDPWD=", 7) == 0)
			printf("minishell: cd: OLDPWD not set\n");
		return (-1);
	}
	(*path) = (*val);
	new_oldpwd = getcwd(NULL, 0);
	if (chdir((*path)) == -1)
	{
		perror("chdir");
		return (-1);
	}
	new_pwd = getcwd(NULL, 0);
	ft_cd_env_update(new_oldpwd, new_pwd, mini);
	return (0);
}

int	ft_cd_tiret(char *oldpwd, char **path, t_mini *mini)
{
	char	new_oldpwd[1024];
	char	new_pwd[1024];

	oldpwd = ft_cd_val_env("OLDPWD=", mini);
	if ((oldpwd) == NULL)
	{
		printf("minishell: cd: OLDPWD not set\n");
		return (-1);
	}
	(*path) = (oldpwd);
	if (getcwd(new_oldpwd, sizeof(new_oldpwd)) == NULL)
		return (perror("minishell: cd"), -1);
	if (chdir((*path)) == -1)
	{
		printf("cd: %s", (*path));
		printf(": No such file or directory\n");
		return (-1);
	}
	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
		return (perror("minishell: cd"), -1);
	ft_cd_env_update(new_oldpwd, new_pwd, mini);
	printf("%s\n", (*path));
	return (0);
}

int	check_cd_tiret_ou_move(char **tab, t_mini *mini, t_cd_buf *buf, char **path)
{
	if (tab[1][0] == '-' && tab[1][1] == '\0')
	{
		if (ft_cd_tiret(NULL, path, mini) == -1)
			return (mini->exit_status = 1);
		return (mini->exit_status = 0);
	}
	if (getcwd(buf->oldpwd, sizeof(buf->oldpwd)) == NULL)
		return (mini->exit_status = 1, perror("minishell: cd"), -1);
	if (chdir(tab[1]) == -1)
		return (mini->exit_status = 1, perror("chdir"), -1);
	if (getcwd(buf->pwd, sizeof(buf->pwd)) == NULL)
		return (mini->exit_status = 1, perror("minishell: cd"), -1);
	return (1);
}

int	ft_cd_all(char **tab, t_mini *mini)
{
	t_cd_buf	buf;
	char		*home;
	char		*path;
	int			resultat;

	resultat = 0;
	if ((tab[1] != NULL) && (tab[2] != NULL))
		return (printf("cd: too many arguments\n"), mini->exit_status = 1);
	if (tab[1] == NULL || (tab[1][0] == '~' && tab[1][1] == '\0'))
	{
		if (ft_cd_sans_av(&home, &path, "HOME=", mini) == -1)
			return (mini->exit_status = 1);
		return (mini->exit_status = 0);
	}
	if (tab[2] == NULL)
	{
		resultat = check_cd_tiret_ou_move(tab, mini, &buf, &path);
		if (resultat <= 0)
			return (resultat);
		ft_cd_env_update(buf.oldpwd, buf.pwd, mini);
		return (mini->exit_status = 0);
	}
	return (mini->exit_status = 0);
}

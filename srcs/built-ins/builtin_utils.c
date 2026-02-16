/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/16 14:19:18 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *cmd)
{
	int			i;
	const char	*bi_table[] = {"cd", "echo", "pwd", "export", "unset", "env",
			"exit", NULL};

	i = 0;
	if (!cmd || !*cmd)
		return (0);
	while (bi_table[i])
	{
		if (!ft_strncmp(cmd, bi_table[i], ft_strlen(bi_table[i]) + 1))
			return (i + 1);
		i++;
	}
	return (0);
}

void	execute_built_in(t_mini *mini, char **cmd, int type)
{
	if (type == T_CD)
	{
		ft_cd_all(cmd, mini);
	}
	else if (type == T_ECHO)
	{
		ft_echo_all(cmd);
	}
	else if (type == T_PWD)
		ft_pwd();
	else if (type == T_EXPORT)
		ft_export_all(cmd, mini);
	else if (type == T_UNSET)
		ft_unset_all(cmd, mini);
	else if (type == T_ENV)
		ft_env(mini);
	else if (type == T_EXIT)
		ft_exit(cmd, mini);
}

void	execute_built_in2(t_mini *mini, char **cmd, int type)
{
	if (type == T_CD)
	{
		ft_cd_all(cmd, mini);
	}
	else if (type == T_ECHO)
	{
		ft_echo_all(cmd);
	}
	else if (type == T_PWD)
		ft_pwd();
	else if (type == T_EXPORT)
		ft_export_all(cmd, mini);
	else if (type == T_UNSET)
		ft_unset_all(cmd, mini);
	else if (type == T_ENV)
		ft_env(mini);
	else if (type == T_EXIT)
		ft_exit2(cmd, mini);
}

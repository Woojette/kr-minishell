/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse_if.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	est_espace(char c)
{
	return (c == ' ' || c == '\t');
}

int	est_redirection(char *line)
{
	return (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2)
		|| !ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1));
}

int	est_pipe(char *line)
{
	return (!ft_strncmp(line, "|", 1));
}

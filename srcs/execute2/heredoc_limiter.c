/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_limiter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:17:03 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/15 17:17:04 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote_limiter(char *limiter)
{
	int		i;
	char	quote;

	i = 0;
	if (!limiter)
		return (0);
	while (limiter[i] != '\0')
	{
		if (limiter[i] == '\'' || limiter[i] == '"')
		{
			quote = limiter[i];
			i++;
			while (limiter[i] && limiter[i] != quote)
				i++;
			if (limiter[i] == quote)
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (0);
}

int	check_heredoc_env(char *limiter)
{
	if (!limiter)
		return (0);
	if (!check_quote_limiter(limiter))
		return (1);
	return (0);
}

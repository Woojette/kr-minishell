/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:40:32 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/15 11:40:33 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_paths(t_mini *mini)
{
	if (mini->path_array)
	{
		free_tab_char(mini->path_array);
		mini->path_array = NULL;
	}
}

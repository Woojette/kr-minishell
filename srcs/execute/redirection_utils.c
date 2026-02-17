/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 06:09:37 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/17 06:09:38 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_save(int in_save, int out_save)
{
	close(in_save);
	close(out_save);
}

int	close_save_exit_status(t_mini *mini, int in_save, int out_save)
{
	close_save(in_save, out_save);
	return (mini->exit_status);
}

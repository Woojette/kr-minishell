/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_temp_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 12:18:21 by yookyeoc          #+#    #+#             */
/*   Updated: 2026/02/15 12:18:22 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*temp_file_name(int j, int n)
{
	char	*index_char;
	char	*temp_j;
	char	*tiret_bas;
	char	*temp_j_n;

	index_char = ft_itoa(j);
	if (!index_char)
		return (perror("index j (itoa)"), NULL);
	temp_j = ft_strjoin("temp_", index_char);
	free(index_char);
	if (!temp_j)
		return (perror("strjoin: temp_ + j"), NULL);
	tiret_bas = ft_strjoin(temp_j, "_");
	free(temp_j);
	if (!tiret_bas)
		return (perror("strjoin: temp_j + _"), NULL);
	index_char = ft_itoa(n);
	if (!index_char)
		return (perror("itoa index n"), free(tiret_bas), NULL);
	temp_j_n = ft_strjoin(tiret_bas, index_char);
	free(tiret_bas);
	free(index_char);
	if (!temp_j_n)
		return (perror("malloc: file name j n"), NULL);
	return (temp_j_n);
}

int	preparer_temp_file_name(t_mini *mini, int j, int n)
{
	char	*temp_j_n;

	if (!mini || j < 0 || j >= mini->nbr_cmd)
		return (-1);
	if (n < 0 || n >= mini->cmd_array[j].compter_heredoc)
		return (-1);
	if (mini->cmd_array[j].temp_heredoc && mini->cmd_array[j].temp_heredoc[n])
	{
		unlink(mini->cmd_array[j].temp_heredoc[n]);
		free(mini->cmd_array[j].temp_heredoc[n]);
		mini->cmd_array[j].temp_heredoc[n] = NULL;
	}
	temp_j_n = temp_file_name(j, n);
	if (!temp_j_n)
		return (-1);
	mini->cmd_array[j].temp_heredoc[n] = temp_j_n;
	if (access(mini->cmd_array[j].temp_heredoc[n], F_OK) == 0)
		unlink(mini->cmd_array[j].temp_heredoc[n]);
	return (0);
}

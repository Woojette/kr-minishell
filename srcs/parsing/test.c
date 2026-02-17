/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yookyeoc <yookyeoc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/14 18:54:23 by yookyeoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

// const char	*token_type_to_str(t_type_token type)
// {
// 	if (type == T_MOT)            return ("T_MOT");
// 	if (type == T_PIPE)           return ("T_PIPE");
// 	if (type == T_RD_IN)          return ("T_RD_IN");
// 	if (type == T_RD_OUT)         return ("T_RD_OUT");
// 	if (type == T_RD_APPEND)      return ("T_RD_APPEND");
// 	if (type == T_RD_HEREDOC)     return ("T_RD_HEREDOC");
// 	if (type == T_FD_IN)          return ("T_FD_IN");
// 	if (type == T_FD_HEREDOC)     return ("T_FD_HEREDOC");
// 	if (type == T_FD_OUT)         return ("T_FD_OUT");
// 	if (type == T_FD_OUT_APPEND)  return ("T_FD_OUT_APPEND");
// 	return ("T_UNKNOWN");
// }

// void	print_tokens(t_token *token)
// {
// 	int	i = 0;

// 	while (token)
// 	{
// 		printf("----- TOKEN %d -----\n", i);
// 		printf("str        : %s\n", token->str ? token->str : "(null)");
// 		printf("type_token : %s (%d)\n",
// 			token_type_to_str(token->type_token), token->type_token);
// 		printf("type_bi    : %d\n", token->type_bi);
// 		printf("type_quote : %d\n", token->type_quote);
// 		printf("next       : %p\n", (void *)token->next);
// 		printf("\n");

// 		token = token->next;
// 		i++;
// 	}
// }

// void	print_tab_char(char **tab)
// {
// 	int	i = 0;

// 	if (!tab)
// 	{
// 		printf("(null)\n");
// 		return ;
// 	}
// 	while (tab[i])
// 	{
// 		printf("[%d] = %s\n", i, tab[i]);
// 		i++;
// 	}
// }

// void	print_tab_int(int *tab, int size)
// {
// 	int	i = 0;

// 	if (!tab)
// 	{
// 		printf("(null)\n");
// 		return ;
// 	}
// 	while (i < size)
// 	{
// 		printf("[%d] = %d\n", i, tab[i]);
// 		i++;
// 	}
// }

// void	print_cmd(t_cmd *cmd, int index)
// {
// 	printf("\n========== CMD %d ==========\n", index);

// 	printf("cmd (arguments):\n");
// 	print_tab_char(cmd->cmd);

// 	printf("\ninoutfile (redir files):\n");
// 	print_tab_char(cmd->inoutfile);

// 	printf("\ntemp_heredoc:\n");
// 	print_tab_char(cmd->temp_heredoc);

// 	printf("\nlimiter:\n");
// 	print_tab_char(cmd->limiter);

// 	printf("\ncompter_heredoc = %d\n", cmd->compter_heredoc);

// 	printf("\nin_heredoc:\n");
// 	print_tab_int(cmd->in_heredoc, cmd->compter_ihoa);

// 	printf("\nihoa (redir types):\n");
// 	print_tab_int(cmd->ihoa, cmd->compter_ihoa);

// 	printf("\ncompter_ihoa = %d\n", cmd->compter_ihoa);

// 	printf("\nheredoc flag = %d\n", cmd->heredoc);

// 	printf("\nhd_env:\n");
// 	print_tab_int(cmd->hd_env, cmd->compter_heredoc);

// 	printf("\npid_heredoc = %d\n", cmd->pid_heredoc);

// 	printf("\nfd_in = %d\n", cmd->fd_in);
// 	printf("fd_out = %d\n", cmd->fd_out);

// 	printf("\ninout_fail = %d\n", cmd->inout_fail);

// 	printf("================================\n");
// }

// void	print_cmd_array(t_cmd *cmd_array, int nbr_cmd)
// {
// 	int	i = 0;

// 	while (i < nbr_cmd)
// 	{
// 		print_cmd(&cmd_array[i], i);
// 		i++;
// 	}
// }

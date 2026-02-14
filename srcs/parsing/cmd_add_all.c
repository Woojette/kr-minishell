/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	appliquer_add_cmd_pipe(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0)
			return (-1);
	if (cmd[var_cmd->index_cmd].inoutfile || cmd[var_cmd->index_cmd].compter_ihoa > 0)
		var_cmd->redir_existe = 1;
	else
		var_cmd->redir_existe = 0;
	if (token->next == NULL) // proteger au cas ou il y a un pipe a la fin (ex: cmd1 | )
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	if (token->next->type_token == T_PIPE) // proteger au cas ou il y a 2 pipes consecutifs (ex: cmd1 || cmd2)
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	if (cmd[var_cmd->index_cmd].cmd == NULL && var_cmd->i == 0 && !var_cmd->redir_existe)
	// proteger au cas ou il y a un pipe au debut (ex: | cmd1 )
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	// if(cmd[var_cmd->index_cmd].cmd == NULL) // proteger au cas ou il y a 2 pipes consecutifs (ex: cmd1 || cmd2)
	// 	return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	if (cmd[var_cmd->index_cmd].cmd != NULL) // si le tableau n'est pas vide 
	// ex) meme s'il y a pas d'argument T_MOT, mais il y a redir et fichier peut-etre
		cmd[var_cmd->index_cmd].cmd[var_cmd->i] = NULL; // on met le NULL terminateur pour cloturer argv
	var_cmd->index_cmd++; // on passe a la structure suivante
	var_cmd->i = 0; // pour reverifier des le debut, on reinitialise l'index pour l'argument de cette nouvelle structure
	return (0);
}

int	appliquer_add_cmd_type(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0)
		return (-1);
	if (token->type_token == T_MOT)
		return (appliquer_add_cmd_mot_all(token, cmd, var_cmd));
	if (token->type_token == T_FD_IN || token->type_token == T_FD_OUT || token->type_token == T_FD_OUT_APPEND)
		return (appliquer_add_cmd_redir(token, cmd, var_cmd));
	if (token->type_token == T_FD_HEREDOC)
		return (appliquer_add_cmd_heredoc_all(token, cmd, var_cmd));
	if (token->type_token == T_PIPE)
		return (appliquer_add_cmd_pipe(token, cmd, var_cmd));
	return (-1);
}

// parcourir les token, et rajouter les token dans les tableaux
// ex) echo hihi | cat -e
// l'objectif, c'est de mettre  tab[0] = {"echo", "hihi", NULL}, tab[1] = {"cat", "-e", NULL}  dans la liste chainee cmd
// ( remplir  cmd[0].cmd = {"echo","hihi",NULL}, cmd[1].cmd = {"cat","-e",NULL} )
int add_cmd(t_token *token, t_cmd *cmd)
{
	t_var_cmd	var_cmd; // structure pour stocker les variables temporaires dans la fonction add_cmd
	int 			resultat;

	init_var_cmd(&var_cmd, &resultat);
	while (token) // pendant que le noeud dans la liste chainee existe
	{
		resultat = appliquer_add_cmd_type(token, cmd, &var_cmd);
		if (resultat < 0)
			return (free_cmd_interieur(cmd, var_cmd.index_cmd + 1), resultat);
		token = token->next; // on passe au noeud suivant
	}
	if (var_cmd.index_cmd > 0 && cmd[var_cmd.index_cmd].cmd == NULL
			&& cmd[var_cmd.index_cmd].inoutfile == NULL && !cmd[var_cmd.index_cmd].heredoc)
	{
		free_cmd_interieur(cmd, var_cmd.index_cmd + 1);
		return (write(2, "Error: syntax error near unexpected token '|'\n", 47), -2);
	}
			// proteger au cas ou il y a un pipe a la fin (ex: cmd1 | )
			// et aussi proteger le cas ou il y a un pipe avec redir mais sans mot (ex: cmd1 | < file)
	if (cmd[var_cmd.index_cmd].cmd != NULL) // on termine aussi le dernier argv (apres la boucle)
		cmd[var_cmd.index_cmd].cmd[var_cmd.i] = NULL; // on ferme bien la fin 
	return (0); // pour distinguer du cas qui marche bien le cas d'erreur (-1)
}

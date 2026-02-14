/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add_mot.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int appliquer_add_cmd_mot_1(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0) // verifier l'index_cmd pour proteger
		return (-1);
	if (cmd[var_cmd->index_cmd].cmd == NULL) // si le tableau cmd[var_cmd->index_cmd].cmd n'est pas encore alloué (NULL)
	{
		cmd[var_cmd->index_cmd].cmd = malloc(sizeof(char *) * 2); 
		// initialement allouer pour 2 cases (tab[0] = "~~" , tab[1] = NULL)
		if (!cmd[var_cmd->index_cmd].cmd) 
			return (-1);
		cmd[var_cmd->index_cmd].cmd[0] = ft_strdup(token->str); // on ajoute le contenu de token a cet argument de la telle structure
		if (!cmd[var_cmd->index_cmd].cmd[0])
			return (free(cmd[var_cmd->index_cmd].cmd), cmd[var_cmd->index_cmd].cmd = NULL, -1);
		cmd[var_cmd->index_cmd].cmd[1] = NULL; // vu que c'est un double tableau (tableau de pointeurs char *, donc argv), on place d'abord le NULL final
	}
  else if (cmd[var_cmd->index_cmd].cmd)
    return (1); // si le tableau cmd[var_cmd->index_cmd].cmd n'est pas NULL, on retourne 1 pour signaler qu'il y a deja un argument dans le tableau, et qu'on doit agrandir le tableau pour ajouter un nouvel argument
  return (0);
}

int appliquer_add_cmd_mot_2(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0) // verifier l'index_cmd pour proteger
		return (-1);
  if (!cmd[var_cmd->index_cmd].cmd) // si le tableau cmd[var_cmd->index_cmd].cmd est NULL, on retourne 0 pour signaler qu'on vient de l'allouer et ajouter le premier argument, et qu'on n'a pas besoin d'agrandir le tableau
    return (-1);
  var_cmd->mot_temp = ft_strdup(token->str); // dupliquer le contenu de token->str (pour proteger)
  if (!var_cmd->mot_temp)
    return (-1);
  var_cmd->size_cmd = len_tab_char(cmd[var_cmd->index_cmd].cmd);
  var_cmd->new_tab_char = add_double_tab_char(cmd[var_cmd->index_cmd].cmd, var_cmd->mot_temp, var_cmd->size_cmd); // agrandir le tableau cmd[var_cmd->index_cmd].cmd pour ajouter le nouveau mot
  // ex) tab[0] = {"echo", NULL} -> {"echo", "hihi", NULL}
  // on le sauvegarde d'abord dans un pointeur temporaire pour proteger au cas ou add_double_tab_char retourne NULL (perte de tous les pointeurs dans cmd[var_cmd->index_cmd].cmd)
  if (!var_cmd->new_tab_char)
    return (free(var_cmd->mot_temp), var_cmd->mot_temp = NULL, -1);
  cmd[var_cmd->index_cmd].cmd = var_cmd->new_tab_char; // mettre a jour le tableau de la structure avec le nouveau tableau agrandi
  return (0);
}

int	appliquer_add_cmd_mot_all(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd)
{
	int resultat_1;

	if (!token || !cmd || !var_cmd || var_cmd->index_cmd < 0) // verifier l'index_cmd pour proteger
		return (-1);
	resultat_1 = appliquer_add_cmd_mot_1(token, cmd, var_cmd);
  if (resultat_1 < 0)
    return (-1);
  else if (resultat_1 == 1)
  {
    if (appliquer_add_cmd_mot_2(token, cmd, var_cmd) < 0)
      return (-1);
  }
	var_cmd->i++; // i = nombre d'arguments actuels (prochain index libre)
	return (0);
}

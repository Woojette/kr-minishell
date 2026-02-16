/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parsing(t_m *m)
{
	m->resultat = check_readline(m);
	if (m->resultat == -2)
		return (-2);
	else if (m->resultat == -1)
		return (-1);
	add_history(m->line);
	m->resultat = check_syntaxe(m);
	if (m->resultat == -1)
		return (-1);
	m->resultat = check_erreur_cmd(m);
	if (m->resultat == -1)
		return (-1);
	else if (m->resultat == -2)
		return (-2);
	m->resultat = check_cmd_heredoc(m);
	if (m->resultat == -1)
		return (-1);
	else if (m->resultat == -2)
		return (-2);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd(t_cmd *cmd, int j)
{
	cmd[j].cmd = NULL; // on initialise tous les pointeurs a NULL (pour proteger)
	// redirections: tableaux (on garde tous les noms de fichiers dans l'ordre)
	// inoutfile 로 리다이렉션 이름 싹다 집어넣을거임 
	cmd[j].inoutfile = NULL; // liste des fichiers pour  < (char **, termine par NULL)
	cmd[j].temp_heredoc = NULL; // tableau (char **) des noms de fichiers temporaire pour heredoc (qui termine par NULL)
	cmd[j].limiter = NULL; // tableau (char **) des limiters pour heredoc (qui termine par NULL)
	cmd[j].compter_heredoc = 0; // pour compter le nombre de heredoc (<<) dans chaque commande
	cmd[j].in_heredoc = NULL; // tableau int pour sauvegarder le type de in redir (<, <<) par ordre
	cmd[j].compter_ihoa = 0; // ihoa 다 세어서 넣을것
	cmd[j].hd_env = NULL; // tableau int: appliquer env expansion pour chaque limiter[n] (1 env, 0 sinon)
	// ihoa 에다 인트 배열로 인아웃파일이랑 똑같은 인덱스에 리다이렉션 타입 넣을거임
	cmd[j].ihoa = NULL; // initialiser a NULL pour proteger // 여기다가 리다이렉션 타입 싹 집어넣을것
	// NULL au debut: on alloue et agrandit au fur et a mesure dans add_cmd()
	// cf) out_append[n] = 1 pour >> (append),  0 pour > (truncate) / 1 4 2 3 순으로 인 히어독 아웃 아펜드
	cmd[j].heredoc = 0; // <<
	cmd[j].pid_heredoc = -1; // pid pour heredoc (fork)
	cmd[j].fd_in = -1;
	cmd[j].fd_out = -1;
	cmd[j].inout_fail = 0;
}

// allouer le tableau de structures cmd (divise par pipe)
// on va mettre chaque commande divisee par pipe dans chaque cmd[i].cmd
t_cmd	*malloc_cmd(t_token *token)
{
	int		compter_pipe; // le nombre de pipes dans la ligne de commande
	int		nbr_cmd; // le nombre de commandes a allouer le memoire
	int		j; // index pour parcourir le tableau de structures cmd
	t_cmd	*cmd; // le tableau de structures pour chaque commande divisee par pipe

	j = 0;
	compter_pipe = count_pipe(token); // compter le nombre de pipes
	nbr_cmd = compter_pipe + 1; // le nombre de commandes = le nombre de pipes + 1 
	// ex) echo "hihi" | cat -e | cat -e  <- 2 pipes, donc 3 commandes
	cmd = malloc(sizeof(t_cmd) * nbr_cmd); // allouer un tableau de structures t_cmd (une par commande)
	if (!cmd)
		return (NULL);
	while (j < nbr_cmd) // j est index, donc ca commence par 0
	{
		init_cmd(cmd, j);
		j++;
	}
	return (cmd);
}

void	init_var_cmd(t_var_cmd *var_cmd, int *resultat)
{
	if (!var_cmd)
		return ;
	*resultat = 0;
	var_cmd->index_cmd = 0; // l'index pour la structure  ex) tab[0] = {"echo", "hihi", NULL}, tab[1] = {"cat", "-e", NULL}
	var_cmd->i = 0; // l'index pour l'argument de chaque structure  ex) tab[0][0] = "echo", tab[0][1] = "hihi", tab[0][2] = NULL
	var_cmd->n = 0; // l'index pour limiters de heredoc
	var_cmd->redir_existe = 0;
	var_cmd->mot_temp = NULL; // temporaire pour le mot
	var_cmd->file_temp = NULL; // temporaire pour le nom de fichier
	var_cmd->size_file_tab = 0; // pour compter la taille actuelle du tableau de fichiers (infile ou outfile) pour agrandir le tableau et ajouter un nouveau fichier
	var_cmd->limiter_sans_quote = 0;
	var_cmd->limiter_env = 0;
	var_cmd->new_tab_char = NULL; // temporaire pour le nom de la commande (pour proteger)
	// si l'un des fonctions add_tab_char ou add_tab_int retourne NULL, ca risque de perdre tous les pointeurs qui etaient deja dans le tableau cmd, donc on utilise des pointeurs temporaires pour proteger (pour garder des anciens pointeurs)
	var_cmd->new_tab_int = NULL; // temporaire pour le tableau int (pour proteger)
	var_cmd->size_cmd = 0;
}

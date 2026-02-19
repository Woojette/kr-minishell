/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 21:25:44 by wooyang           #+#    #+#             */
/*   Updated: 2026/02/18 21:25:48 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include <errno.h>

extern int	g_exit_status;

typedef enum s_state
{
	DQUOTES,
	SQUOTES,
	GENERAL,
}	t_state;

typedef enum s_type_token
{
	T_MOT,
	T_PIPE,
	T_RD_IN,
	T_RD_OUT,
	T_RD_APPEND,
	T_RD_HEREDOC,
	T_FD_IN,
	T_FD_HEREDOC,
	T_FD_OUT,
	T_FD_OUT_APPEND,
}	t_type_token;

typedef enum s_type_bi
{
	T_NOT_BUILT_IN,
	T_CD,
	T_ECHO,
	T_PWD,
	T_EXPORT,
	T_UNSET,
	T_ENV,
	T_EXIT,
}	t_type_bi;

typedef enum s_redirect
{
	DEFAULT,
	TRUNC,
	APPEND,
	INF,
	HEREDOC,
}	t_redirect;

typedef struct s_quote
{
	int	i;
	int	n;
	int	s_quote;
	int	d_quote;
}		t_quote;

typedef struct s_var_cmd
{
	int		index_cmd;
	int		i;
	int		n;
	int		redir_existe;
	char	*mot_temp;
	char	*file_temp;
	int		size_file_tab;
	char	*limiter_sans_quote;
	int		limiter_env;
	char	**new_tab_char;
	int		*new_tab_int;
	int		size_cmd;
}			t_var_cmd;

typedef struct s_cd_buf
{
	char	oldpwd[1024];
	char	pwd[1024];
}	t_cd_buf;

typedef struct s_unset
{
	int		j;
	int		supprime;
	int		taille;
	int		len_str;
	char	**env_supprime;
}	t_unset;

typedef struct s_redir
{
	int			n;
	int			k;
	int			type;
	const char	*path;
}				t_redir;

typedef struct s_token
{
	char			*str;
	t_type_token	type_token;
	t_type_bi		type_bi;
	t_state			type_quote;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	**cmd;
	char	**inoutfile;
	char	**temp_heredoc;
	char	**limiter;
	int		compter_heredoc;
	int		*in_heredoc;
	int		*ihoa;
	int		compter_ihoa;
	int		heredoc;
	int		*hd_env;
	pid_t	pid_heredoc;
	int		fd_in;
	int		fd_out;
	int		inout_fail;
}	t_cmd;

typedef struct s_mini
{
	char			**env;
	int				exit_status;
	t_cmd			*cmd_array;
	int				nbr_cmd;
	int				pipe_read_end;
	char			**path_array;
	struct termios	orig_term;
	char			**save_ex;
	void			*m_ptr;
}	t_mini;

typedef struct s_m
{
	char			*line;
	t_cmd			*cmd;
	t_token			*parsing;
	int				j;
	int				resultat;
	int				nbr_cmd;
	t_mini			*mini;
}	t_m;

typedef struct s_hd_enfant
{
	t_mini *mini;
	int	j;
	int	n;
	int	fd_temp;
	int	resultat;
}	t_hd_enfant;



// ======================================================= token =======================================================


// cmd_add_all.c
int			appliquer_add_cmd_pipe(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);
int			appliquer_add_cmd_type(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);
int			add_cmd(t_token *token, t_cmd *cmd);

// cmd_add_heredoc.c
int			appliquer_inoutfile_ihoa(t_cmd *cmd, t_var_cmd *var_cmd);
int			appliquer_add_cmd_heredoc_1(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);
int			appliquer_add_cmd_heredoc_2(t_cmd *cmd, t_var_cmd *var_cmd);
int			appliquer_add_cmd_heredoc_all(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);

// cmd_add_mot.c
int			appliquer_add_cmd_mot_1(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);
int			appliquer_add_cmd_mot_2(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);
int			appliquer_add_cmd_mot_all(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);

// cmd_add_redir.c
int			add_cmd_redir_if(t_token *token, t_cmd *cmd, t_var_cmd *var_cmd);
int			appliquer_add_cmd_redir(t_token *token, t_cmd *cmd,
				t_var_cmd *var_cmd);

// cmd_agrandi.c
char		**add_double_tab_char(char **tab, char *str, int size);
int			*add_double_tab_int(int *tab, int val, int size);

// cmd_init.c
void		init_cmd(t_cmd *cmd, int j);
t_cmd		*malloc_cmd(t_token *token);
void		init_var_cmd(t_var_cmd *var_cmd, int *resultat);

// env_appliquer.c
int			appliquer_dollar_sur_liste_token(t_token **token, t_mini *mini);
int			appliquer_quote_sur_liste_token(t_token **token);

// env_dollar.c
void		init_val(int *i, int *s_quote, int *d_quote, char **resultat);
char		*char_et_quote(char *resultat, int *i, int *quote, char *str);
char		*remplacer_dollar(char *str, t_mini *mini);

// env_var.c
char		*get_env_name(char *str, int start);
char		*get_env_var(char *str, t_mini *mini);
char		*ajouter_char(char *resultat, char c);
char		*appliquer_env_var(char *resultat, char *str, t_mini *mini, int *i);

// free_cmd.c
void		free_cmd_tab_char(t_cmd *cmd);
void		free_cmd_fd_tab_sans_hd(t_cmd *cmd); // 서로 맞추기 위해 조정 필요
void		free_cmd_partiel(t_cmd *cmd, int nbr_cmd);
void		free_cmd_interieur(t_cmd *cmd, int nbr_cmd);
void		free_cmd_all(t_cmd *cmd, int nbr_cmd);

// free_parsing.c
void		free_temp_heredoc(char **temp);
void		free_mini(t_mini *mini); // 서로 맞추기 위해 조정 필요

// free_token_tab.c
void		free_tokens(t_token **token);
void		free_tab_char(char **tab);
void		free_tab_int(int *tab);

// pipe_check.c
int			check_pipe_fin(char *line);
int			count_pipe(t_token *token);

// quote_enlever.c
void		inverser_et_incrementer(int *quote, int *i);
void		init_enlever_quote(t_quote *quote);
void		quote_a_enlever(char *str, char *resultat, t_quote *quote);
char		*enlever_quote_dans_token(char *str);

// signaux.c
void		init_signaux(void);
void		appliquer_sigint_prompt(int sig);
void		print_heredoc_warning_ctrl_d(char *delimiter);
void		set_exit(int *exit_status);
// token_all.c
int			traiter_redirection(char **line, t_token **token,
				t_type_token *fd_type);
int			traiter_pipe(char **line, t_token **token, t_type_token *fd_type);
int			traiter_mot(char **line, t_token **token, t_type_token *fd_type,
				int *len);
int			traiter_token(char **line, t_token **token, t_type_token *fd_type,
				int *len);
int			parse_input(char *line, t_token **token, t_mini *mini);
// token_len_mot_all.c
int			appliquer_quote_premier(char *line, int *len);
int			appliquer_quote_milieu(char *line, int *len);
int			len_mot_total(char *line);

// token_len_mot_etc.c
int			len_mot_2_quotes_entier(char *line);
int			len_mot_apres_quote(char *line);
int			len_tab_char(char **tab);
int			len_mot_sans_quote(char *line);
int			len_mot_avant_quote(char *line);

// token_parse_if.c
int			est_espace(char c);
int			est_redirection(char *line);
int			est_pipe(char *line);

// token_parse_redir.c
int			appliquer_redir_2_len(char **line, t_token **token,
				t_type_token *fd_type);
int			appliquer_redir_1_len(char **line, t_token **token,
				t_type_token *fd_type);
int			appliquer_redir_token(char **line, t_token **token,
				t_type_token *fd_type);

// token_parse_type.c
void		passer_espace(char **line);
int			appliquer_pipe_token(char **line, t_token **token,
				t_type_token *fd_type);
int			appliquer_mot_token(char **line, t_token **token,
				t_type_token *fd_type, int *len);
int			appliquer_token_final(t_token **token, t_type_token fd_type,
				t_mini *mini);

// token_quote_char.c
char		caractere_quote_debut(char *line);
int			index_quote_debut(char *line, char c);
int			index_quote_fin(char *line, char c);

// token_quote_position.c
int			check_quote_debut_ok(char *line);
int			check_2_quotes_debut_puis_fin(char *line);
int			check_quote_fermee(char *line, int debut_quote,
				int compte_debut_quote);
int			check_quote_milieu_ok(char *line);

// token_quotes.c
int			check_quotes(char *line);
int			check_avant_quote_espace(char *line);
int			check_2_quotes_milieu_puis_fin(char *line);

// token_type.c
int			new_node_init(t_token **new_node, char *line, int len,
				t_type_token type_token);
int			add_token(char *line, t_type_token type_token, int len,
				t_token **token);
void		appliquer_token_fd(t_token *temp);
void		parse_fd_tokens(t_token **token);
const char	*get_token_type_str(t_type_token type);

// env_expansion.c
char		*get_env_name(char *str, int start);
char		*get_env_var(char *str, t_mini *mini);
char		*ajouter_char(char *resultat, char c);
char		*appliquer_env_var(char *resultat, char *str, t_mini *mini, int *i);
char		*remplacer_dollar(char *str, t_mini *mini);
int			appliquer_dollar_sur_liste_token(t_token **token, t_mini *mini);
int			appliquer_quote_sur_liste_token(t_token **token);

// signaux.c
void		init_signaux(void);
void		appliquer_sigint_prompt(int sig);
void		print_heredoc_warning_ctrl_d(char *delimiter);
void		set_signal_exec_child(void);
void		set_signal_parent_wait(void);

const char	*get_token_type_str(t_type_token type);

//main_parsing
int			check_parsing(t_m *m);
int			check_readline(t_m *m);
int			check_syntaxe(t_m *m);
int			check_erreur_cmd(t_m *m);
int			check_cmd_heredoc(t_m *m);
void		erreur_syntaxe(t_m *m, int code);
void		erreur_parse(t_m *m);
void		erreur_cmd(t_m *m);
int			check_erreur_syntaxe(t_m *m);
int			check_erreur_parse_input(t_m *m);
int			init_main(int ac, char **av, char **env, t_m *m);
void		init_avant_prompt(t_m *m);

//clean_utils.c
void		cleanup_paths(t_mini *mini);

// control_c.c
void		control_c(t_mini *mini);
void		termios_back(t_mini *mini);

// dups.c
void		dup_check(t_mini *mini, int in_save, int out_save);
void		p_dup2(t_mini *mini, int fd_has, int fd_to);
void		c_dup2(t_mini *mini, int fd_has, int fd_to);
void		ft_close(int fd);

// env_exp_utils.c
int			ft_env_len(char **env);
void		copy_env(t_mini *mini, char **env);
void		copy_to_exp(t_mini *mini, char **env);
void		copy_env_exp(t_mini *mini, char **env);

// error_utils.c
void		fatal_error(t_mini *mini, char *err);
void		p_exit(t_mini *mini);
void		p_exit_nb(t_mini *mini, int nb_exit);
void		child_exit(t_mini *mini);
void		child_exit_nb(t_mini *mini, int nb_exit);

// execute_manager.c
void		invalid_cmd(t_mini *mini, char *not_cmd);
void		ft_execute(t_mini *mini, t_cmd *cmd);
void		no_path(t_mini *mini, char *cmd);

// file_utils.c
int			is_directory(char *file_path);
int			is_executable(char *file_path);
int			does_file_exist(char *file_path);
int			cmd_qqpart(t_mini *mini);

void		free_round(t_mini *mini);

// heredoc_collect_lines.c
char		*util_hd_line(t_cmd *cmd, t_mini *mini, int n, char *line);
int			write_line(int fd, char *str);
int			while_collect_lines(int fd, t_mini *mini, int j, int n);
int			collecter_heredoc_lines(int fd, t_mini *mini, int j, int n);
int			cut_while_c_l(void);

// heredoc_enfant.c
void		util_struct_check(t_mini *mini, int j, int n);
void		util_close_exit(t_mini *mini, int fd_temp, int exit_flag);
void		sig_hd(t_mini *mini);
void		perexit(t_mini *mini, int exit_flag);
void		appliquer_heredoc_enfant(t_hd_enfant *he);
void		hd_child_cleanup(t_mini *mini, int fd_temp);

// heredoc_enfat_util.c
void		sig_hd_handler(int sig);

// heredoc_limiter.c
int			check_quote_limiter(char *limiter);
int			check_heredoc_env(char *limiter);

// heredoc_refac.c
int			before_appliquer(t_mini *mini, int j);
void		reduce_1(t_mini *mini, int j);
void		reduce_2(t_mini *mini, int j, int status);
int			work_appliquer(t_mini *mini, int j, int n, t_hd_enfant *he);
int			appliquer_heredoc_cmd(t_mini *mini, int j);
int			cut_wa(t_mini *mini, int j, int exit_status);
//heredoc_refac_util.c
void	struct_hdenfant(t_hd_enfant *he, t_mini *mini, int j, int n);

// heredoc_temp_file.c
char		*temp_file_name(int j, int n);
int			preparer_temp_file_name(t_mini *mini, int j, int n);

// path_utils.c
char		*get_env_path(t_mini *mini);
void		set_path_array(t_mini *mini, char *str);
char		*get_path_absolute(t_mini *mini, char *cmd);
char		*get_path_absolute(t_mini *mini, char *cmd);
char		*get_path_envp(t_mini *mini, char *cmd);
char		*cmd_path_center(t_mini *mini, char *cmd);

// process_manager_p_c.c
void		child_center(t_mini *mini, t_cmd cmd, int *pipe_fd, int i);
void		parent_center(t_mini *mini, int pipe_fd[2], int i);

// process_manager_util_child.c
void		before(t_mini *mini, t_cmd cmd);
void		first(t_mini *mini, int *pipe_fd);
void		middle(t_mini *mini, int *pipe_fd);
void		last(t_mini *mini);
void		set_pipe_exit(t_mini *mini, int *pipe_fd);

// process_manager.c
void		fork_center(t_mini *mini);

// redir_inout_refac_util.c
void		heredoc_restore(t_mini *mini, int i, int k);
int			get_redir_type(t_mini *mini, int i, int n);
int			fail_redir(t_mini *mini, int i, t_redir *redir, int inhd);
void		open_fail(t_mini *mini, int i, const char *p, int inhd);
int			before_inout(t_mini *mini, int i);

// redir_inout_refac.c
int			open_redir(t_mini *mini, int i, t_redir *redir);
void		remplacer_fd(int *dst, int fd);
int			inout_redir(t_mini *mini, int i);

// redirection.c
int			redirection_center(t_mini *mini);
void		apply_redirection_child(t_mini *mini, t_cmd *cmd);
void		obar_util(t_mini *mini, int flag);
int			one_builtin_avec_redirs(t_mini *mini);

// redirection_utils.c
void		close_save(int in_save, int out_save);
int			close_save_exit_status(t_mini *mini, int in_save, int out_save);

// built-ins
//builtin_utils.c
int			is_built_in(char *cmd);
void		execute_built_in(t_mini *mini, char **cmd, int type);
void		execute_built_in2(t_mini *mini, char **cmd, int type);
// cd
char		*ft_cd_val_env(char *str, t_mini *mini);
int			ft_cd_sans_av(char **val, char **path, char *str, t_mini *mini);
int			ft_cd_tiret(char *oldpwd, char **path, t_mini *mini);
int			ft_cd_env_update(char *oldpwd, char *pwd, t_mini *mini);
int			ft_cd_all(char **tab, t_mini *mini);
//echo
int			ft_echo_option_n(char *str);
void		ft_echo_all(t_mini *mini, char **tab);
//pwd
int			ft_pwd(void);
//export
int			first_checker(char c);
int			key_letters(char *str);
int			key_len(char *str);
int			equal_checker(char *str);
int			has_valid_key(char *str);
int			has_samekey(char *str, t_mini mini);
int			has_samekey2(char *str, t_mini *mini);
int			same_checker(char *str, t_mini mini);
void		exp_update(char *str, t_mini *mini);
void		env_update(char *str, t_mini *mini);
void		exp_update2(char *str, t_mini *mini, int change_pos);
int			key_index(char *str, char **tab);
void		env_update_sub(char *str, t_mini *mini, int change_pos);
void		env_update2(char *str, t_mini *mini);
void		ex_and_env(char *str, t_mini *mini);
void		re_ex_and_env(char *str, t_mini *mini); // 어떤 줄을 수작칠지 보는 것
void		save_export(char *str, t_mini *mini);
void		bubble_pointer(char **str1, char **str2);
void		export_sort(t_mini *mini);
void		export_print(t_mini *mini); // 아씨발 따옴표 까먹음
int			ft_export_all(char **tab, t_mini *mini);

//env
void		ft_env(t_mini *mini);
int			ft_check_env_egal(char *str);
int			ft_check_env_double(char *str, t_mini *mini);
//unset
int			ft_unset_all(char **tab, t_mini *mini);
int			ft_unset(char *str, t_unset *u, t_mini *mini);
int			ft_unset_init(char *str, t_unset *u, t_mini *mini);
int			ft_unset2(char *str, t_unset *u, t_mini *mini);
int			ft_unset2_init(char *str, t_unset *u, t_mini *mini);
int			ft_unset_path(char *str, t_mini *mini);
//exit
void		ft_exit_sans_arg(t_mini *mini);
void		ft_exit_normal_arg(long long val, t_mini *mini);
void		ft_exit_pl_arg(t_mini *mini);
void		ft_exit_wrong_arg(char *str, t_mini *mini);
void		ft_exit(char **tab, t_mini *mini);
void		exit_clean(t_mini *mini);

//exit child
void		ft_exit_sans_arg2(t_mini *mini);
void		ft_exit_normal_arg2(long long val, t_mini *mini);
void		ft_exit_wrong_arg2(char *str, t_mini *mini);
void		ft_exit2(char **tab, t_mini *mini);

//exit_ft
int			ft_exit_check_not_int(char *str);
long long	ft_exit_atoi_long(const char *str, int *error);

//bi_free
void		ft_free_tab(char **tab);
void		ft_free_all(t_mini **mini);

// main exec
void		execution_main(t_mini *mini);
void		clean_after_exec(t_mini *mini, t_token *parsing, char *line);
void		clean_after_cmd(t_m *m);
int			after_all(t_mini *mini);

#endif
#ifndef MODEL_H

# define MODEL_H

# include <unistd.h>
# include <sys/wait.h>

// extern int	g_exit_status;

enum
{
	REDIR_OUT,        // >
	REDIR_APPEND,     // >>
	REDIR_IN,         // <
	HEREDOC           // <<
};

typedef struct s_redirect {
	int					type;
	char				*filename;	
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd_family {
	char		            **cmd;
	t_redirect          *redirect;
	struct s_cmd_family *next;
}	t_cmd_family;

// cmd[0] = 'echo'
// cmd[1] = 'hello'


// typedef struct s_data {
// 	int		process_number;
// 	int		prev_fd;
// 	char	**path;
// 	char	**my_env;
// }	t_data;

t_cmd_family *build_echo_cat_family(void);


#endif
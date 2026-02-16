NAME = minishell

CC = cc
INC_DIR 	= includes
SRC_DIR 	= srcs
OBJ_DIR     = objs
LIBFT_DIR 	= libft

CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_DIR)
CFLAGS  += -g3 -O0 -fno-omit-frame-pointer
LIBS = -lreadline -lncurses $(LIBFT_DIR)/libft.a

SRCS_FILES = 	minishell_main.c \
				execute/heredoc_refac.c \
				execute/free_parsing_refac.c \
				execute/env_exp_utils.c \
				execute/redir_inout_refac.c \
				execute/redir_inout_refac_util.c \
				execute/execute_manager.c \
				execute/process_manager.c \
				execute/error_utils.c \
				execute/main_refac_exec.c \
				execute/path_utils.c \
				execute/file_utils.c \
				execute/redirection.c \
				execute/control_c.c \
				execute/clean_utils.c \
				execute/dups.c \
				execute/heredoc_enfant.c \
				execute/heredoc_limiter.c \
				execute/heredoc_temp_file.c \
				execute/heredoc_collect_lines.c \
				execute/process_manager_p_c.c \
				execute/process_manager_util_child.c \
				built-ins/builtin_utils.c \
				built-ins/bi_cd.c \
				built-ins/bi_echo.c \
				built-ins/bi_pwd.c \
				built-ins/bi_export.c \
				built-ins/bi_export_utils1.c \
				built-ins/bi_export_utils2.c \
				built-ins/bi_export_utils3.c \
				built-ins/bi_export_utils4.c \
				built-ins/bi_export_utils5.c \
				built-ins/bi_free.c \
				built-ins/bi_env.c \
				built-ins/bi_unset.c \
				built-ins/bi_exit.c \
				built-ins/bi_exit_child.c \
				built-ins/bi_exit_ft.c \
				parsing/env_appliquer.c \
				parsing/env_dollar.c \
				parsing/env_var.c \
				parsing/quote_enlever.c \
				parsing/signaux.c \
     			parsing/token_all.c \
				parsing/token_parse_if.c \
				parsing/token_parse_redir.c \
				parsing/token_parse_type.c \
				parsing/token_type.c \
    			parsing/token_quote.c \
				parsing/token_quote_position.c \
				parsing/token_quote_char.c \
				parsing/free_token_tab.c \
				parsing/free_parsing.c \
				parsing/free_cmd.c \
 				parsing/pipe_check.c \
				parsing/token_len_mot_all.c \
				parsing/token_len_mot_etc.c \
				parsing/cmd_add_all.c \
				parsing/cmd_add_heredoc.c \
				parsing/cmd_add_mot.c \
				parsing/cmd_add_redir.c \
				parsing/cmd_agrandi.c \
				parsing/test.c \
				parsing/cmd_init.c


SRCS = $(addprefix $(SRC_DIR)/, $(SRCS_FILES))
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean fclean re

all: $(NAME)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

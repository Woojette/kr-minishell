NAME = minishell

CC = cc
INC_DIR 	= includes
SRC_DIR 	= srcs
OBJ_DIR     = objs
LIBFT_DIR 	= libft

CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_DIR)
LIBS = -lreadline -lncurses $(LIBFT_DIR)/libft.a

SRCS_FILES = 	minishell_main.c \
				parsing/env_appliquer.c \
				parsing/env_dollar.c \
				parsing/env_var.c \
				parsing/heredoc.c \
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
				parsing/redir_in_out.c \
 				parsing/test_parsing.c \
				parsing/token_len_mot_all.c \
				parsing/token_len_mot_etc.c \
				parsing/cmd_add_all.c \
				parsing/cmd_add_heredoc.c \
				parsing/cmd_add_mot.c \
				parsing/cmd_add_redir.c \
				parsing/cmd_add_agrandi.c \
				parsing/cmd_add_init.c

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

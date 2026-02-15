# ════════════════════════════════════════════════════════════════════════════ #
#                           CONFIGURATION VARIABLES                            #
# ════════════════════════════════════════════════════════════════════════════ #

NAME = minishell_exec

CC = cc
CFLAGS = -Wall -Wextra -Werror  -I$(INC_DIR) -I$(LIBFT_DIR)
LIBS = -lreadline -lncurses $(LIBFT_DIR)/libft.a 

INC_DIR 	= includes
SRC_DIR 	= .
OBJ_DIR     = objs

LIBFT_DIR 	= ../../libft

# ════════════════════════════════════════════════════════════════════════════ #
#                                SOURCE FILES                                  #
# ════════════════════════════════════════════════════════════════════════════ #

SRCS =  main_execute.c \
		execute_manager.c \
		process_manager.c \
		error_utils.c \
		path_utils.c \
		file_utils.c \
		hardcode.c 
# ════════════════════════════════════════════════════════════════════════════ #
#                                OBJECT FILES                                  #
# ════════════════════════════════════════════════════════════════════════════ #

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

# ════════════════════════════════════════════════════════════════════════════ #
#                                PHONY TARGETS                                 #
# ════════════════════════════════════════════════════════════════════════════ #

.PHONY: all clean fclean re

# ════════════════════════════════════════════════════════════════════════════ #
#                                DEFAULT TARGET                                #
# ════════════════════════════════════════════════════════════════════════════ #

all: $(NAME)

# ════════════════════════════════════════════════════════════════════════════ #
#                                 BUILD RULES                                  #
# ════════════════════════════════════════════════════════════════════════════ #

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# ════════════════════════════════════════════════════════════════════════════ #
#                                CLEANUP RULES                                 #
# ════════════════════════════════════════════════════════════════════════════ #

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) 

re	: fclean all

NAME		= libft.a

SRCS		= ft_isalpha.c\
			  ft_isdigit.c\
		  	  ft_isalnum.c\
			  ft_isascii.c\
			  ft_isprint.c\
			  ft_strlen.c\
			  ft_memset.c\
			  ft_bzero.c\
			  ft_memcpy.c\
			  ft_memmove.c\
			  ft_strlcpy.c\
			  ft_strlcat.c\
			  ft_toupper.c\
			  ft_tolower.c\
			  ft_strchr.c\
			  ft_strrchr.c\
			  ft_strncmp.c\
			  ft_strcmp.c\
			  ft_strndup.c\
			  ft_memchr.c\
			  ft_memcmp.c\
			  ft_strnstr.c\
			  ft_atoi.c\
			  ft_calloc.c\
			  ft_strdup.c\
			  ft_substr.c\
			  ft_strjoin.c\
			  ft_strtrim.c\
			  ft_split.c\
			  ft_putchar_fd.c\
			  ft_putstr_fd.c\
			  ft_striteri.c\
			  ft_itoa.c\
			  ft_strmapi.c\
			  ft_putendl_fd.c\
			  ft_putnbr_fd.c\
			  ft_lstnew_bonus.c\
			  ft_lstadd_front_bonus.c\
			  ft_lstadd_back_bonus.c\
			  ft_lstsize_bonus.c\
			  ft_lstlast_bonus.c

CC = cc
OBJS = ${SRCS:.c=.o}
RM = rm -f
CFLAGS = -Wall -Werror -Wextra
AR = ar rcs

%.o : %.c
	$(CC) $(CFLAGS) -I includes -c $< -o $@

$(NAME) : $(OBJS)
	$(AR) $(NAME) $(OBJS)

clean :
	$(RM) $(OBJS) $(BONUS_OBJS)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re bonus

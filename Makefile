NAME = minishell

SRCS = main.c \
		lexer/lexer_utils.c \
		lexer/lexer.c \
		lexer/utils.c \
		lexer/clean.c \
		lexer/init.c \
		lexer/expand.c \
		lexer/utils2.c \
		lexer/expand_utils.c \
		parser/parser.c \
		parser/parser_utils.c 
OBJS = $(SRCS:.c=.o)


INCLUDES = -I include
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline -fsanitize=address

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
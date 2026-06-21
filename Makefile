NAME = minishell

SRCS =	lexer/lexer.c \
		lexer/lexer_utils.c \
		lexer/lexer_utils2.c \
		lexer/utils.c \
		lexer/clean.c \
		lexer/clean2.c \
		lexer/init.c \
		lexer/expand.c \
		lexer/utils2.c \
		lexer/expand_utils.c \
		parser/parser.c \
		parser/parser_utils.c \
		src/main.c \
		src/init.c \
		src/here_doc.c \
		src/exc_external.c \
		src/exc_external_2.c \
		src/pipe_working.c \
		src/pipe_working_2.c \
		src/pipe_working_3.c \
		src/redirections.c \
		builtin/builtin.c \
		builtin/builtin_cd.c \
		builtin/builtin_export.c \
		builtin/builtin_export_2.c \
		builtin/builtin_pwd_env_unset_exit.c \
		builtin/buitin_echo.c \
		builtin/env_transactions.c \
		lib/ft_add_env_node.c \
		lib/ft_atol.c \
		lib/ft_bubblesort.c \
		lib/ft_bzero.c \
		lib/ft_check_env.c \
		lib/ft_command_count.c \
		lib/ft_copy_env_list.c \
		lib/ft_free_cmd_list.c \
		lib/ft_free_pipes.c \
		lib/ft_free_split.c \
		lib/ft_isalnum.c \
		lib/ft_isalpha.c \
		lib/ft_isappend.c \
		lib/ft_join_and_free.c \
		lib/ft_lstlast.c \
		lib/ft_redir_error.c \
		lib/ft_safe_dup2.c \
		lib/ft_split.c \
		lib/ft_strcmp.c \
		lib/ft_strdup.c \
		lib/ft_strjoin.c \
		lib/ft_strlcpy.c \
		lib/ft_strlcat.c \
		lib/ft_strlen.c \
		lib/ft_substr.c \
		lib/is_valid_exit_args.c \
		lib/is_valid_indentifier.c \
		src/get_next_line/get_next_line.c \
		src/get_next_line/get_next_line_utils.c 

OBJS = $(SRCS:.c=.o)

INCLUDES = -I include 
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME) 
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
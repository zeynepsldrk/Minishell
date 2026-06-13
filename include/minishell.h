/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 22:01:03 by asay              #+#    #+#             */
/*   Updated: 2026/06/13 04:43:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_PATH 1024
# define KEY 1
# define VALUE 2
# define ERR_CMD_NOT_FOUND 127
# define ERR_CANNOT_EXEC 126

# define ERANGE	34

extern int g_signal;

typedef struct s_minishell t_shell;
typedef struct s_builtin t_builtin;

typedef enum e_token_type
{
	WORD = 1,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND
} t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*context;
	struct s_token	*next;
	int				expand;
	int 			is_joined;
} t_token;

typedef struct s_redirect
{
	int				heredoc_fd[2];
	t_token_type	type;
	char			*target;
	struct s_redirect *next;
} t_redirect;

typedef struct s_cmd
{
	int			argc;
	char		**argv;
	struct s_cmd *next;
	t_redirect	*redirects;
} t_cmd;

typedef struct s_builtin
{
	char	*name;
	int		(*func)(t_shell *shell, int in_pipe);
} t_builtin;

typedef struct s_env_node
{
	char			*key;
	char			*value;
	int				has_value;
	struct s_env_node *next;
} t_env_node;

typedef struct s_pipe
{
	int		pipe_count;
	int		command_count;
	int		**fd;
} t_pipe;

typedef struct s_minishell
{
	t_pipe		pipes;
	t_env_node	*env_list;
	char		**env;
	char		*input;
	int			exit_value;
	t_token		*tokens;
	t_builtin	list_builtin[8];
	t_cmd		*cmds;
} t_shell;

typedef struct s_lexer
{
	int				i;
	int				j;
	char			*buff;
	t_token			*head;
	t_token			*tail;
	t_token			*curr;
	t_token_type	value;
	int				in_single;
	int				in_double;
	int				has_quote;
} t_lexer;

typedef struct s_expander
{
	int		i;
	char	*str;
	char	*key;
	char	*value;
	char	*old;
	t_token	*curr_token;
} t_expander;

// lexer
t_token			*new_token(t_token_type type, char *context);
t_token_type	get_value(char *str, int *i);
t_token			*get_tokens(char *str);
int				pipe_ctrl(char *str, int i);
int				lexer(t_shell *shell, char *str);
void			whitespace_tkn(t_lexer *ptr, char *str);
void			redirect_tkn(t_lexer *ptr, char *str);
void			quote_tkn(t_lexer *ptr, char *str);
void			pipe_tkn(t_lexer *ptr, char *str);
int				lexer_init(t_lexer *ptr, char *str);
void			add_token(t_token **head, t_token *new);
void			clean_get_tkns(t_lexer *ptr);
void			is_gonna_expand(t_lexer *lex, t_token *tkn);
void			get_token_helper(t_lexer *lex);
void			general_quote_handler(t_lexer *ptr, char *str);
void join_tokens(t_shell *shell);

// expander
void			expander(t_shell *sh);
void			expander_helper(t_shell *sh, t_expander *exp);
char			*ch_value(t_expander *exp);
char			*get_env_key(char *str, int i);
char			*get_env_value(t_shell *sh, char *key);
void			handle_exitval(t_shell *sh, t_expander *exp);

// parser
void			parser(t_shell *sh);
t_cmd			*get_cmds(t_shell *sh);
void			fill_cmd(t_cmd *cmd, t_token *token);
int				handle_pipe(t_cmd **cmd, t_token **token);
char			**get_argv(t_token *token);
t_redirect		*get_redirs(t_token *token);
int				word_count(t_token *token);

// utils
char			**copy_env(char **env);
int				sh_init(t_shell *shell, char **env);
void			free_str(char **str);
void			free_sh(t_shell *shell);
void			free_tokens(t_token *token);
void			free_commands(t_cmd *cmds);
void			*ft_memset(void *b, int c, size_t len);
int				ft_strncmp(char *str1, char *str2, int n);
char			*trim(char *str);
int				numlen(long nbr);
char			*ft_itoa(int nbr);
char			*ft_strdup(char *s);
int ft_strlen(const char *str);


// src/main.c
int				main(int argc, char **argv, char **envp);
void			lets_start_shell(t_shell *shell);
void			start_execute(t_shell *shell);
t_cmd			*start_parser(char *input, t_shell *shell);
void			works_ctrl_c(int signal);

// src/init.c
t_shell			*init_shell(t_shell *shell, char **envp);
void			init_builtins(t_builtin *builtins);

// src/exc_external.c
char			*ft_join_and_free(char *s1, char *s2);
void			ft_free_split(char **arr);
char			*check_the_path(char *path, char *cmd);
void			print_path_error(t_shell *shell, char *cmd, char *msg, int exit_code);
int				is_path_okey(t_shell *shell, char *path);
int				is_absolute_path(char *path);
char			*find_command_path(t_shell *shell);
void			execute_external(t_shell *shell, int in_pipe);

// src/pipe_working.c
int				**create_pipes(int pipe_count);
void			wait_for_children(t_shell *shell, int *how_died, pid_t *pid);
void			pipe_working(t_shell *shell);

// src/pipe_working_2.c
void			execute_command(t_shell *shell);
void			connect_child_fds(int i, int cmd_count, int **fd);
void			execute_child_logic(t_shell *shell, t_cmd *cmd, int i);
void			spawn_commands(t_shell *shell, pid_t *pid, int i);

// src/redirections.c
int				apply_redirect_out(t_redirect *redir);
int				apply_redirect_in(t_redirect *redir);
int				apply_heredoc(t_redirect *redir);
int				apply_append(t_redirect *redir);
int				apply_redir(t_redirect *redir);

// builtin/builtin.c
void			execute_builtin(char *cmd, t_shell *shell, int i, int in_pipe);
int				is_builtin(char *cmd, t_shell *shell);
char			*my_little_getenv(t_env_node *env_list, char *key);

// builtin/builtin_cd.c
char			*cd_special_control(t_shell *shell);
int				builtin_cd(t_shell *shell, int in_pipe);

// builtin/buitin_echo.c
int				info_flags(char *arg);
int				builtin_echo(t_shell *shell, int in_pipe);

// builtin/builtin_pwd_env_unset_exit.c
int				builtin_pwd(t_shell *shell, int in_pipe);
int				builtin_env(t_shell *shell, int in_pipe);
int				builtin_unset(t_shell *shell, int in_pipe);
int				builtin_exit(t_shell *shell, int in_pipe);

// builtin/builtin_export.c
void			handle_append(t_shell *shell, char *key, char *new_val);
void			execute_export(t_shell *shell, char *key, char *value, int i);
void			only_export_command(t_env_node *env_list);
int				process_export_arg(t_shell *shell, int i);
int				builtin_export(t_shell *shell, int in_pipe);

// builtin/builtin_export_2.c
int				is_key_inside(char *key, t_env_node *env_list);
char			*find_key_or_value(char *arg, int i, int which_one);

// builtin/env_transactions.c
int				update_env_node(char *key, char *value, t_shell *shell);
int				create_new_node(t_shell *shell, char *key, char *value);
int				delete_env_node(t_env_node **env_list, char *key);
int				print_env_list(t_env_node *env_list);

// lib
void			add_env_node(t_env_node **env_list, t_env_node *new_node);
long			ft_atol(const char *str);
void			swap(t_env_node *temp, int *swapped);
void			ft_bubble_sort(t_env_node **env_list, int swapped);
void			ft_bzero(void *p, int num);
void			ft_check_env(t_shell *shell);
int				ft_command_count(t_cmd *cmd);
t_env_node		*ft_copy_env_list(t_env_node *env_list);
void			ft_free_cmd_list(t_cmd *cmds);
void			ft_free_pipes(int **fd, int count);
int				ft_isalpha(int c);
int				ft_isalnum(char c);
int				ft_isappend(char *arg);
t_env_node		*ft_lstlast(t_env_node *env_list);
int				ft_redir_error(int fd);
int				ft_safe_dup2(int fd, int file_no);
int				ft_word_count(char const *s, char c);
char			*ft_word_dup(char const *s, char c);
void			ft_free_all(char **arr, int i);
char			**ft_split_fill(char **result, char const *s, char c);
char			**ft_split(char const *s, char c);
int				ft_strcmp(char *s1, char *s2);
char			*ft_safe_strdup(char *s);
void			ft_strlcpy(char *dst, const char *src, size_t size);
void			ft_strlcat(char *dst, const char *src, size_t size);
char			*ft_strjoin(char const *s1, char const *s2);
int				is_valid_exit_arg(const char *str);
int				is_valid_identifier(char *arg);
char	*ft_substr(char *s,int start, int len);

#endif

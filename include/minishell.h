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

extern int g_signal; // diğer dosyalrdan okunabilmesi için 

typedef struct s_minishell t_shell;
typedef struct s_builtin t_builtin;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
} t_token;
	
typedef struct s_redirect
{
	int	heredoc_fd[2];
	t_token_type type;
	char *target_file;
	struct s_redirect *next;
} t_redirect;

typedef struct s_cmd
{
	int argc;
	char **argv;
	struct s_cmd *next;
	t_redirect *redirects;
} t_cmd;

typedef struct s_builtin
{
    char *name;
    int (*func)(t_shell *shell, int in_pipe);
} t_builtin;

typedef struct s_env_node
{
    char *key;
    char *value;
    int has_value; //kullanılmıyor
    struct s_env_node *next;
} t_env_node;

typedef struct s_pipe
{
    int pipe_count;
    int command_count;
    int **fd;
} t_pipe;

typedef struct s_minishell
{
    t_pipe pipes;
	t_env_node	*env_list;
	char		**env;
	char		*input;
	int			exit_value;
	t_token		*tokens;
	t_builtin	list_builtin[8];
	t_cmd		*cmds;
} t_shell;

// src/main.c
int main(int argc, char **argv, char **envp);
void lets_start_shell(t_shell *shell);
void start_execute(t_shell *shell);
t_cmd *start_parser(char *input, t_shell *shell);
void works_ctrl_c(int signal);

// src/init.c
t_shell	*init_shell(t_shell *shell, char **envp);
void	init_builtins(t_builtin *builtins);

// src/exc_external.c
char *ft_join_and_free(char *s1, char *s2);
void ft_free_split(char **arr);
char *check_the_path(char *path, char *cmd);
void print_path_error(char *cmd, char *msg, int exit_code);
int is_path_okey(char *path);
int is_absolute_path(char *path);
char *find_command_path(t_shell *shell);
void execute_external(t_shell *shell, int in_pipe);

// src/pipe_working.c
int **create_pipes(int pipe_count);
void wait_for_children(t_shell *shell, int *how_died, pid_t *pid);
void pipe_working(t_shell *shell);

// src/pipe_working_2.c
void execute_command(t_shell *shell);
void connect_child_fds(int i, int cmd_count, int **fd);
void execute_child_logic(t_shell *shell, t_cmd *cmd, int i);
void spawn_commands(t_shell *shell, pid_t *pid, int i);

// src/redirections.c
int apply_redirect_out(t_redirect *redir);
int apply_redirect_in(t_redirect *redir);
int apply_heredoc(t_redirect *redir);
int apply_append(t_redirect *redir);
int apply_redir(t_redirect *redir);

// builtin/builtin.c
void execute_builtin(char *cmd, t_shell *shell, int i, int in_pipe);
int is_builtin(char *cmd, t_shell *shell);
char *my_little_getenv(t_env_node *env_list, char *key);

// builtin/builtin_cd.c
char *cd_special_control(t_shell *shell);
int builtin_cd(t_shell *shell);

// builtin/buitin_echo.c
int print_with_escapes(char *str, int i);
int info_flags(char *arg, int *n_flag, int *e_flag);
int builtin_echo(t_shell *shell);

// builtin/builtin_pwd_env_unset_exit.c
int builtin_pwd(t_shell *shell);
int builtin_env(t_shell *shell);
int builtin_unset(t_shell *shell);
int builtin_exit(t_shell *shell, int in_pipe);

// builtin/builtin_export.c
void handle_append(t_shell *shell, char *key, char *new_val);
void execute_export(t_shell *shell, char *key, char *value, int i);
void only_export_command(t_env_node *env_list);
int process_export_arg(t_shell *shell, int i);
int builtin_export(t_shell *shell);

// builtin/builtin_export_2.c
int is_key_inside(char *key, t_env_node *env_list);
char *find_key_or_value(char *arg, int i, int which_one);

// builtin/env_transactions.c
int update_env_node(char *key, char *value, t_shell *shell);
int create_new_node(t_shell *shell, char *key, char *value);
int delete_env_node(t_env_node **env_list, char *key);
int print_env_list(t_env_node *env_list);

// lib/ft_add_env_node.c
void add_env_node(t_env_node **env_list, t_env_node *new_node);

// lib/ft_atol.c
long ft_atol(const char *str);

// lib/ft_bubblesort.c
void swap(t_env_node *temp, int *swapped);
void ft_bubble_sort(t_env_node **env_list, int swapped);

// lib/ft_bzero.c
void ft_bzero(void *p, int num);

// lib/ft_check_env.c
void ft_check_env(t_shell *shell);

// lib/ft_command_count.c
int ft_command_count(t_cmd *cmd);

// lib/ft_copy_env_list.c
t_env_node *ft_copy_env_list(t_env_node *env_list);

// lib/ft_free_pipes.c
void ft_free_pipes(int **fd, int count);

// lib/ft_isalnum.c
int ft_isalnum(int c);

// lib/ft_isalpha.c
int ft_isalpha(int c);

// lib/ft_isappend.c
int ft_isappend(char *arg);

// lib/ft_lstlast.c
t_env_node *ft_lstlast(t_env_node *env_list);

// lib/ft_redir_error.c
int ft_redir_error(int fd);

// lib/ft_safe_dup2.c
int ft_safe_dup2(int fd, int file_no);

// lib/ft_split.c
int ft_word_count(char const *s, char c);
char *ft_word_dup(char const *s, char c);
void ft_free_all(char **arr, int i);
char **ft_split_fill(char **result, char const *s, char c);
char **ft_split(char const *s, char c);

// lib/ft_strcmp.c
int ft_strcmp(char *s1, char *s2);

// lib/ft_strdup.c
char *ft_strdup(const char *s);
char *ft_safe_strdup(char *s);

// lib/ft_strlcpy.c
void ft_strlcpy(char *dst, const char *src, size_t size);
// lib/ft_strlcat.c
void ft_strlcat(char *dst, const char *src, size_t size);

// lib/ft_strlen.c
int ft_strlen(const char *str);

// lib/ft_strjoin.c
char *ft_strjoin(char const *s1, char const *s2);

// lib/ft_substr.c
char *ft_substr(char const *s, int start, size_t len);

// lib/is_valid_exit_args.c
int is_valid_exit_arg(const char *str);

// lib/is_valid_indentifier.c
int is_valid_identifier(char *arg);

#endif
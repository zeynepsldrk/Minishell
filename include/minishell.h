#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_PATH 1024
# define KEY 1
# define VALUE 2

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
    int (*func)(t_shell *shell);
} t_builtin;

typedef struct s_env_node
{
    char *key;
    char *value;
    int has_value; //kullanılmıyor
    struct s_env_node *next;
} t_env_node;

typedef struct s_minishell
{
	t_env_node	*env_list;
	char		**env;
	char		*input;
	int			exit_value;
	t_token		*tokens;
	t_builtin	list_builtin[8];
	t_cmd		*cmds;
} t_shell;



t_cmd	*start_parser(char *input, t_shell *shell);

t_shell	*init_shell(t_shell *shell, char **envp);
void	lets_start_shell(t_shell *shell);
void	start_execute(t_shell *shell);

char *my_little_getenv(t_env_node *env_list, char *key);
int	is_builtin(char *cmd, t_shell *shell);
int	builtin_cd(t_shell *shell);
int	builtin_echo(t_shell *shell);
int	builtin_exit(t_shell *shell);
int	builtin_pwd(t_shell *shell);
int	builtin_export(t_shell *shell);
int	builtin_unset(t_shell *shell);
int	builtin_env(t_shell *shell);

void	ft_bzero(void *p, size_t num);
int ft_isalpha(int c);
int ft_isalnum(int c);
char	*ft_strdup(const char *s);
int		ft_str_cmp(char *s1, char *s2);
int		ft_str_len(char *str);
int		ft_redir_error(int fd);

int		print_with_escapes(char *str, int i);
int		info_flags(char *arg, int *n_flag, int *e_flag, int *any_flag_here);
void	*execute_builtin(char *cmd, t_shell *shell, int i);
void	apply_redir(t_redirect *redirects, t_shell *shell);

void pipe_working(t_shell *shell);
void execute_external(t_shell *shell);

char *find_key_or_value(char *arg, int i, int which_one);
int is_valid_for_export(char **args, int i, int j);
int is_key_inside(char *key, t_env_node *env_list);


void	update_env_node(char *key, char *value, t_shell *shell);
void add_env_node(t_env_node **env_list, t_env_node *new_node);
void delete_env_node(t_env_node **env_list, char *key, char *value);
void print_env_list(t_env_node *env_list);
void create_new_node(t_shell *shell, char *key, char *value);

#endif
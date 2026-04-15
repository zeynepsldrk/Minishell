#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

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
    int has_value;
    struct s_env_node *next;
} t_env_node;

typedef struct s_minishell
{
    t_env_node *env_list;
	char **env;
    char *input;
	int exit_value;
	t_token *tokens;
    t_builtin *builtins;
    t_cmd *cmds;
} t_shell;



t_cmd *start_parser(char *input, t_shell *shell);

t_shell *init_shell(t_shell *shell, char **envp);
void lets_start_shell(t_shell *shell);
void start_execute(t_shell *shell);


int is_builtin(char *cmd, t_shell *shell);
int builtin_cd(t_shell *shell);
int builtin_echo(t_shell *shell);
int builtin_exit(t_shell *shell);
int builtin_pwd(t_shell *shell);
int builtin_export(t_shell *shell);
int builtin_unset(t_shell *shell);
int builtin_env(t_shell *shell);

#endif
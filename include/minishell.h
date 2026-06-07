/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 22:01:03 by asay              #+#    #+#             */
/*   Updated: 2026/06/07 19:32:19 by marvin           ###   ########.fr       */
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

extern int g_signal; // diğer dosyalrdan okunabilmesi için 

typedef struct s_minishell t_shell;
typedef struct s_builtin t_builtin;

//önce WORD'e printable olmayan bir ascii değerini atadık. 
//sonraki gelenler yapı gereği 2, 3 olarak otomatik artıyor.
//enum default olarak 0'dan başlar. 0 = NULL çakışması yaşanmasın diye printable olmayan aralıktan başlıyoruz. 
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
	t_token_type type;
	char *context;
	struct s_token *next;
	int expand;
} t_token;
	
typedef struct s_redirect
{
	int	heredoc_fd[2];
	t_token_type type;
	char *target;
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
	char **env;
	int exit_value;
	t_token *tokens;
} t_shell;

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 22:01:03 by asay              #+#    #+#             */
/*   Updated: 2026/04/02 22:17:27 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

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
} t_token;
	
typedef struct s_redirect
{
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

typedef struct s_minishell
{
	char **env;
	char *input;
	int exit_value;
	t_token *tokens;
	t_cmd *cmds;
} t_shell;

typedef struct s_lexer
{
	int i;
    int j;
    char *buff;
    t_token *head;
    t_token *curr;
    t_token_type value;
	int in_single;
	int in_double;
} t_lexer;

// int	is_quote_open(char *str);
t_token			*new_token(t_token_type type, char *context);
t_token_type	get_value(char *str, int *i);
t_token			*get_tokens(char *str);
int				pipe_ctrl(char *str, int i);
int				ft_strlen(char *str);
char			*ft_strdup(char *str);
char 			*trim(char *str);
int				prompt(t_shell *shell); //readline basmalı
char			*ft_substr(char *str, int start, int len);
t_token 		*get_token_util(char *str, t_token **curr, int *i, char *buff);
char 			**copy_env(char **env);
int				sh_init(t_shell *shell, char **env);
void    		free_str(char **str);
void			free_sh(t_shell *shell);
int				lexer(t_shell *shell, char *str);
void 			whitespace_tkn(t_lexer *ptr, char *str);
void 			redirect_tkn(t_lexer *ptr, char *str);
void 			quote_tkn(t_lexer *ptr, char *str);
void 			pipe_tkn(t_lexer *ptr, char *str);
int				lexer_init(t_lexer *ptr, char *str);




#endif
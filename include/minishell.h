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
	int expand;
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
	int has_quote;
} t_lexer;

typedef struct s_expander
{
	int 	i;
	char 	*str;
	char 	*key;
	char 	*value;
	char 	*old;
	t_token *curr_token;
} t_expander;

t_token			*new_token(t_token_type type, char *context);
t_token_type	get_value(char *str, int *i);
t_token			*get_tokens(char *str);
int				pipe_ctrl(char *str, int i);
int				ft_strlen(char *str);
char			*ft_strdup(char *str);
char 			*trim(char *str);
int				prompt(t_shell *shell); //readline basmalı
char			*ft_substr(char *str, int start, int len);
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
void			add_token(t_token **head, t_token *new);
void			*ft_memset(void *b, int c, size_t len);
void			free_tokens(t_token *token);
void			free_commands(t_cmd *cmds);
void			clean_get_tkns(t_lexer *ptr);
void			is_gonna_expand(t_lexer *lex, t_token *tkn);
void			get_token_helper(t_lexer *lex);
int				ft_isalnum(char c);
char			*get_env_key(char *str, int i);
char			*get_env_value(t_shell *sh, char *key);
int				ft_strncmp(char *str1, char *str2, int n);
void 			expander_helper(t_shell *sh, t_expander *exp);
char 			*ch_value(t_expander *exp);
int				word_count(t_token *token);
void			expander(t_shell *sh);
void			fill_cmd(t_cmd *cmd, t_token *token);
char **get_argv(t_token *token);
t_redirect *get_redirs(t_token *token);
int				handle_pipe(t_cmd **cmd, t_token **token);
void parser (t_shell *sh);
int numlen(long nbr);
char *ft_itoa(int nbr);
void handle_exitval(t_shell *sh, t_expander *exp);

#endif

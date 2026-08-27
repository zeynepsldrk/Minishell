/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:38:00 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 20:50:38 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	whitespace_tkn(t_lexer *ptr, char *str)
{
	if (ptr->buff[0] || ptr->has_quote)
	{
		ptr->curr = new_token(WORD, ptr->buff);
		if (ptr->is_heredoc)
		{
			ptr->curr->expand = 0;
			ptr->is_heredoc = 0;
		}
		else
			is_gonna_expand(ptr->curr, ptr->in_single);
		add_token(&ptr->head, ptr->curr);
		ft_memset(ptr->buff, 0, ptr->j);
		ptr->j = 0;
		ptr->has_quote = 0;
	}
	while ((str[ptr->i] == 32 || str[ptr->i] == '\t') && str[ptr->i])
		ptr->i++;
}

static void	set_redirect_token(t_lexer *ptr)
{
	if (ptr->value == APPEND)
		ptr->curr = new_token(ptr->value, ">>");
	else if (ptr->value == HEREDOC)
	{
		ptr->curr = new_token(ptr->value, "<<");
		ptr->is_heredoc = 1;
	}
	else if (ptr->value == REDIRECT_OUT)
		ptr->curr = new_token(ptr->value, ">");
	else
		ptr->curr = new_token(ptr->value, "<");
	add_token(&ptr->head, ptr->curr);
}

void	redirect_tkn(t_lexer *ptr, char *str)
{
	if (ptr->buff[0] || ptr->has_quote)
	{
		ptr->curr = new_token(WORD, ptr->buff);
		is_gonna_expand(ptr->curr, ptr->in_single);
		add_token(&ptr->head, ptr->curr);
		ft_memset(ptr->buff, 0, ptr->j);
		ptr->j = 0;
		ptr->has_quote = 0;
	}
	set_redirect_token(ptr);
	if (ptr->value == APPEND || ptr->value == HEREDOC)
		ptr->i++;
	ptr->i++;
	while (str[ptr->i] == 32)
		ptr->i++;
	ft_memset(ptr->buff, 0, ptr->j);
	ptr->j = 0;
}

void	quote_tkn(t_lexer *ptr, char *str)
{
	char	quote_type;

	ptr->has_quote = 1;
	quote_type = str[ptr->i];
	if (quote_type == 34)
		ptr->in_double = 1;
	if (quote_type == 39)
		ptr->in_single = 1;
	ptr->i++;
	while (str[ptr->i] && str[ptr->i] != quote_type)
		ptr->buff[ptr->j++] = str[ptr->i++];
	if (str[ptr->i] == quote_type)
		ptr->i++;
	else
	{
		ptr->syntax = 1;
		write(2, "minishell: syntax error: unclosed quote\n", 40);
	}
}

void	pipe_tkn(t_lexer *ptr, char *str)
{
	if (ptr->buff[0] || ptr->has_quote)
	{
		ptr->curr = new_token(WORD, ptr->buff);
		is_gonna_expand(ptr->curr, ptr->in_single);
		add_token(&ptr->head, ptr->curr);
		ft_memset(ptr->buff, 0, ptr->j);
		ptr->j = 0;
		ptr->has_quote = 0;
	}
	ptr->curr = new_token(ptr->value, "|");
	add_token(&ptr->head, ptr->curr);
	ptr->i++;
	while (str[ptr->i] == 32)
		ptr->i++;
	ft_memset(ptr->buff, 0, ptr->j);
	ptr->j = 0;
}

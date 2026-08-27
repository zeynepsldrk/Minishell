/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 17:52:47 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 20:10:35 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lexer(t_shell *shell, char *str)
{
	t_token	*tokens;
	t_token	*temp;

	tokens = get_tokens(str);
	if (!tokens)
		return (0);
	temp = tokens;
	while (temp)
	{
		temp = temp->next;
	}
	shell->tokens = tokens;
	return (1);
}

void	get_token_helper(t_lexer *lex, int in_single)
{
	t_token	*current;

	if (lex->j > 0 || lex->has_quote)
	{
		current = new_token(WORD, lex->buff);
		if (lex->is_heredoc)
		{
			current->expand = 0;
			lex->is_heredoc = 0;
		}
		else
			is_gonna_expand(current, in_single);
		add_token(&lex->head, current);
		lex->tail = current;
	}
}

static void	process_char(t_lexer *ptr, char *str)
{
	if (str[ptr->i] == 32 || str[ptr->i] == '\t')
		whitespace_tkn(ptr, str);
	else if (str[ptr->i] == '<' || str[ptr->i] == '>')
		redirect_tkn(ptr, str);
	else if (str[ptr->i] == 34 || str[ptr->i] == 39)
	{
		if (ptr->j > 0)
		{
			get_token_helper(ptr, 0);
			ptr->tail->is_joined = 1;
			ptr->j = 0;
			ft_memset(ptr->buff, 0, ft_strlen(ptr->buff) + 1);
		}
		general_quote_handler(ptr, str);
	}
	else if (str[ptr->i] == '|')
		pipe_tkn(ptr, str);
	else
	{
		if (str[ptr->i] == '$' && str[ptr->i + 1] == '"')
			ptr->i++;
		else
			ptr->buff[ptr->j++] = str[ptr->i++];
	}
}

t_token	*get_tokens(char *str)
{
	t_lexer	*ptr;
	t_token	*copy_of_head;

	ptr = malloc(sizeof(t_lexer));
	if (!ptr || lexer_init(ptr, str) == -1)
		return (NULL);
	while (str[ptr->i])
	{
		ptr->value = get_value(str, &(ptr->i));
		process_char(ptr, str);
	}
	if (ptr->syntax)
	{
		free_tokens(ptr->head);
		clean_get_tkns(ptr);
		return (NULL);
	}
	get_token_helper(ptr, ptr->in_single);
	copy_of_head = ptr->head;
	clean_get_tkns(ptr);
	return (copy_of_head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 17:51:36 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 17:51:36 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	general_quote_handler(t_lexer *ptr, char *str)
{
	int	in_single;

	quote_tkn(ptr, str);
	in_single = ptr->in_single;
	ptr->in_single = 0;
	ptr->in_double = 0;
	if (!str[ptr->i] || str[ptr->i] == 32 || str[ptr->i] == '|'
		|| str[ptr->i] == '<' || str[ptr->i] == '>')
	{
		get_token_helper(ptr, in_single);
		ptr->j = 0;
		ft_memset(ptr->buff, 0, ft_strlen(ptr->buff) + 1);
		ptr->has_quote = 0;
		ptr->j = 0;
	}
	else
	{
		get_token_helper(ptr, in_single);
		ptr->tail->is_joined = 1;
		ptr->j = 0;
		ft_memset(ptr->buff, 0, ft_strlen(ptr->buff) + 1);
		ptr->has_quote = 0;
	}
	ptr->in_single = 0;
	ptr->in_double = 0;
}

void	join_tokens(t_shell *shell)
{
	t_token	*curr;
	char	*joined;
	t_token	*next_tmp;

	curr = shell->tokens;
	while (curr != NULL && curr->next != NULL)
	{
		if (curr->type == WORD && curr->next->type == WORD && curr->is_joined)
		{
			joined = ft_join_and_free(curr->context, curr->next->context);
			curr->context = joined;
			next_tmp = curr->next;
			curr->is_joined = curr->next->is_joined;
			curr->next = curr->next->next;
			free(next_tmp->context);
			free(next_tmp);
		}
		else
			curr = curr->next;
	}
}

t_token	*new_token(t_token_type type, char *context)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->context = ft_strdup(context);
	token->next = NULL;
	token->is_joined = 0;
	token->expand = 0;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_token_type	get_value(char *str, int *i)
{
	if (str[*i] == '>')
	{
		if (str[*i + 1] == '>')
			return (APPEND);
		return (REDIRECT_OUT);
	}
	else if (str[*i] == '<')
	{
		if (str[*i + 1] == '<')
			return (HEREDOC);
		return (REDIRECT_IN);
	}
	else if (str[*i] == '|')
		return (PIPE);
	else
		return (WORD);
}

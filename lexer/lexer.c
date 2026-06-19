/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:37:52 by asay              #+#    #+#             */
/*   Updated: 2026/06/19 16:22:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int lexer(t_shell *shell, char *str)
{
    t_token *tokens;

    tokens = get_tokens(str);
    if(!tokens)
        return (0);
    //tokenleri sırayla yazdırıp kontrol et.
    t_token *temp = tokens;
    while(temp)
    {
        temp = temp->next;
    }
    //test bitti.
    shell->tokens = tokens;
    return (1);
}

t_token	*new_token(t_token_type type, char *context)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->type = type;
	token->context = ft_strdup(context);
	token->next = NULL;
    token->is_joined = 0;
	return (token);
}

void add_token(t_token **head, t_token *new)
{
	t_token *temp;

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
void get_token_helper(t_lexer *lex)
{
	t_token *current;

	if (lex->j > 0 || lex->has_quote) //eger has_quote quote_tkn sonrası 1 kalmıssa ve bufferde herhangi bir sey varsa """" gibi tokenlarda tek bir bos token ekleyebilsin
    {
		current = new_token(WORD, lex->buff);
		is_gonna_expand(lex, current);
		add_token(&lex->head, current);
        lex->tail = current;
	}
}

t_token *get_tokens(char *str)
{
    t_lexer *ptr;
    t_token *copy_of_head;

    ptr = malloc(sizeof(t_lexer));
    if  (!ptr || lexer_init(ptr, str) == -1)
        return NULL;
    while (str[ptr->i])
    {
        ptr->value = get_value(str, &(ptr->i));
        if (str[ptr->i] == 32)
            whitespace_tkn(ptr, str);
        else if (str[ptr->i] == '<' || str[ptr->i] == '>')
            redirect_tkn(ptr, str);
        else if (str[ptr->i] == 34 || str[ptr->i] == 39)
            general_quote_handler(ptr, str);
        else if (str[ptr->i] == '|')
            pipe_tkn(ptr, str);
        else
            ptr->buff[ptr->j++] = str[ptr->i++];
    }
    get_token_helper(ptr);  
    copy_of_head = ptr->head;
    clean_get_tkns(ptr);
    return (copy_of_head);
}


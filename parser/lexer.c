/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:37:52 by asay              #+#    #+#             */
/*   Updated: 2026/04/25 21:42:31 by asay             ###   ########.fr       */
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
        printf("Type: %d, Context: '%s'\n", temp->type, temp->context);
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
	return (token);
}

//kontrol edildi.
//gelen inputta str[*i]'nin ne olduğunu kontrol ediyoruz.
t_token_type	get_value(char *str, int *i) 
{
	if (str[*i] == '>')
	{
		if(str[*i+1] == '>')
			return (APPEND);
		return (REDIRECT_OUT);
	}
	else if (str[*i] == '<')
	{
		if(str[*i+1] == '<')
			return(HEREDOC);
		return (REDIRECT_IN);
	}
	else if(str[*i] == '|')
		return (PIPE);
	else
		return (WORD);
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
            quote_tkn(ptr, str);
        else if (str[ptr->i] == '|')
            pipe_tkn(ptr, str);
        else
            ptr->buff[ptr->j++] = str[ptr->i++];
    }
    copy_of_head = ptr->head;
    clean_get_tkns(ptr);
    return (copy_of_head);
}

void is_gonna_expand(t_lexer *lex, t_token *tkn)
{
    int     i;
    t_token *new;

    i = 0;
    new = new_token(WORD, lex->buff);
    if (lex->j > 0 || lex->has_quote) //eger has_quote quote_tkn sonrası 1 kalmıssa ve bufferde herhangi bir sey varsa """" gibi tokenlarda tek bir bos token ekleyebilsin
        add_token(&lex->head, new);  
    if (lex->in_double || new->type == WORD)
    {
        while (new->context[i])
        {
            if (new->context[i] == '$')
                new->expand = 1;
            i++;
        }
    }
    else
        new->expand = 0;
}
// YAPILACAKLAR
// is_gonna_expand; lexer sonrası ve parser öncesi yeni token eklemeden kontrol edilecek. 
// WORD token'ı olanların çift ve tek tırnak olmaları durumuna göre expand bayrağını setlicez. 
// bu aşama her token için gerçekleşecek o yüzden her add_token öncesi çağrılmalı.
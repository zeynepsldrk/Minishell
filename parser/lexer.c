/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:37:52 by asay              #+#    #+#             */
/*   Updated: 2026/04/02 22:30:39 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int lexer(t_shell *shell, char *str)
{
    t_token *tokens;

    (void)shell;
    tokens = get_tokens(str);
    if(!tokens)
        return (0);
    //tokenleri sırayla yazdırıp kontrol et.
    return (1);
}

void whitspace_tkn(t_lexer *ptr, char *str)
{
    if(ptr->buff[0])
        {
            ptr->curr = new_token(ptr->value, ptr->buff);
            add_token(&ptr->head, ptr->curr);
            ptr->buff[0] = '\0';
            ptr->j = 0;
            ptr->i++;
        }
        else
        {
            while(str[ptr->i] == 32)
                ptr->i++;
        }
}

void redirect_tkn(t_lexer *ptr, char *str)
{
    
}

void quote_tkn(t_lexer *ptr, char *str)
{
    
}

void pipe_tkn(t_lexer *ptr, char *str)
{
    
}

t_token *get_tokens(char *str)
{
    t_lexer *ptr;

    ptr = malloc(sizeof(t_lexer));
    if(!ptr)
        return NULL;
    ptr->i = 0;
    ptr->j = 0;
    ptr->head = NULL;
    ptr->buff = malloc(sizeof(char) * (ft_strlen(str) + 1));
    if(!ptr->buff)
        return NULL;
    while(str[ptr->i])
    {
        ptr->value = get_value(str, &(ptr->i));
        if(str[ptr->i] == 32)
            whitsepace_tkn(ptr, str);
        else if(str[ptr->i] == '<' || str[ptr->i] == '>')
            redirect_tkn(ptr, str);
        else if(str[ptr->i] == 34 || str[ptr->i] == 39)
            quote_tkn(ptr, str);
        else if(str[ptr->i] == '|')
            pipe_tkn(ptr, str);
        else
            ptr->buff[ptr->j++] = str[ptr->i++];
    }
    return (ptr->head);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:37:52 by asay              #+#    #+#             */
/*   Updated: 2026/04/01 20:52:51 by asay             ###   ########.fr       */
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

t_token *get_tokens(char *str)
{
    int i;
    int j;
    char *buff;
    t_token *head;
    t_token *curr;
    t_token_type value;

    i = 0;
    j = 0;
    head = NULL;
    buff = malloc(sizeof(char) * (ft_strlen(str) + 1));
    if(!buff)
        return NULL;
    while(str[i])
    {
        value = get_value(str, &i);
        if(str[i] == 32 || str[i] == '<' || str[i] == '>'
            || str[i] == 34 || str[i] == 39 || str[i] == '|')
        {
            if(!buff[0])
                i++;
            else
            {
                curr = new_token(value, buff);
                //add_token(&head, curr); //add_token daha yazılmadı. 
                buff[0] = '\0';
                j = 0;
                i++;
            }
        }
        else
            buff[j++] = str[i++];
    }
    return (head);
}


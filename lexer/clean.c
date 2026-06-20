/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 21:22:11 by asay              #+#    #+#             */
/*   Updated: 2026/06/20 14:22:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_str(char **str)
{
    int i;

    i = 0;
    if (!str)
        return ;
    while(str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void clean_get_tkns(t_lexer *lex)
{
    free(lex->buff);
    free(lex);
}

void free_tokens(t_token *token)
{
    t_token *tmp;

    while(token)
    {
        tmp = token;
        token = token->next;
        if(tmp->context)
            free(tmp->context);
        free(tmp);
    }
}

void free_redirects(t_redirect *rdr)
{
    t_redirect *tmp;

    while(rdr)
    {
        tmp = rdr;
        rdr = rdr->next;
        if(tmp->target)
            free(tmp->target);
        free(tmp);
    }
    //açık olan fd'lerin kapatılması gerek!! unutmayinn    
}

void free_commands(t_cmd *cmds)
{
    t_cmd *tmp;

    while (cmds)
    {
        tmp = cmds;
        cmds = cmds->next;
        if (tmp->argv)
            free_str(tmp->argv); // char **argv temizliği
        if (tmp->redirects)
            free_redirects(tmp->redirects); // t_redirect listesi temizliği
        free(tmp);
    }
}

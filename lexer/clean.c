/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 21:22:11 by asay              #+#    #+#             */
/*   Updated: 2026/04/02 22:02:18 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_str(char **str)
{
    int i;

    i = 0;
    while(str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void free_tokens(t_token *token)
{
    t_token *ptr;

    while(token)
    {
        ptr = token;
        token = token->next;
        if(ptr->context)
            free(ptr->context);
        free(ptr);
    }
    ptr = NULL;
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
        //if (tmp->redirects)
            //free_redirects(tmp->redirects); // t_redirect listesi temizliği
        free(tmp);
    }
}

void clean_get_tkns(t_lexer *ptr)
{
    free(ptr->buff);
    free(ptr);
}

void free_sh(t_shell *shell)
{
    free_str(shell->env);
    free_tokens(shell->tokens);
    free_commands(shell->cmds);
    free(shell);
}

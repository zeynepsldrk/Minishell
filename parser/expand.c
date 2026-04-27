/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:05:32 by asay              #+#    #+#             */
/*   Updated: 2026/04/27 17:34:51 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void is_gonna_expand(t_lexer *lex, t_token *tkn)
{
    int     i;

    i = 0;
	tkn->expand = 0;
    if (!lex->in_single)
    {
        while (tkn->context[i])
        {
            if (tkn->context[i] == '$')
                tkn->expand = 1;
            i++;
        }
    }
    else
        tkn->expand = 0;
}



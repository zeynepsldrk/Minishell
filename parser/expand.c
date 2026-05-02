/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:05:32 by asay              #+#    #+#             */
/*   Updated: 2026/05/02 22:30:17 by asay             ###   ########.fr       */
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

void expander(t_shell *sh)
{
    t_expander exp;

    exp.curr_token = sh->tokens;
    while(exp.curr_token)
    {
        exp.i = 0;
        exp.str = exp.curr_token->context;
        while(exp.str[exp.i])
        {
            expander_helper(sh, &exp);
            exp.i++; 
        }
        exp.curr_token = exp.curr_token->next;
    }
}

void expander_helper(t_shell *sh, t_expander *exp)
{
    if(exp->curr_token->expand == 1 && exp->str[exp->i] == '$')
    {
        exp->i++;
        exp->key = get_env_key(exp->str, exp->i);
        exp->value = get_env_value(sh, exp->key);
        if (!exp->key || !exp->value)
        {
            free(exp->key);
            return ;
        }
        exp->old = exp->curr_token->context;
        exp->curr_token->context = ch_value(exp->str, exp->key, exp->value); 
        exp->str = exp->curr_token->context; //bir sonraki döngü için curr_token güncellenmeli
        exp->i = -1; // 0 yapsaydık while sonundaki i++ ile str[0] atlanırdı.
        free(exp->old);
        free(exp->key);
        free(exp->value);
    }
}
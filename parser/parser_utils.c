/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 15:28:39 by asay              #+#    #+#             */
/*   Updated: 2026/06/05 17:05:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
word_count'un yapmasi gerekenler:
WORD → devam et, say
REDIR → devam et, sayma
REDIR'dan sonraki target WORD → devam et, sayma
PIPE → dur
NULL → dur
*/
int word_count(t_token *token)
{
    int count;

    count = 0;
    while (token != NULL && token->type != PIPE) // pipe gorunce cmd'nin sonuna gelmis oluruz
    {
        if(token->type == WORD) // sadece WORD tipindeki token'lari saymamiz gerekiyor
            count++;
        if(token->type == REDIRECT_IN || token->type == REDIRECT_OUT || token->type == HEREDOC || token->type == APPEND)
        {
            if(token->next == NULL) // eger redirect'in targeti yoksa donguden cik
                break; 
            token = token->next; // redir sonrasi target word'u saymayalim diye
        }
        token = token->next;
    }
    return (count);
}

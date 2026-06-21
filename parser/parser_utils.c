/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 15:28:39 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 15:18:05 by zedurak          ###   ########.fr       */
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

void fill_cmd(t_cmd *cmd, t_token *token)
{
    cmd->argv = get_argv(token); 
    cmd->redirects = get_redirs(token);
    cmd->argc = word_count(token);
    cmd->next = NULL;
}

int handle_pipe(t_cmd **cmd, t_token **token)
{
    (*cmd)->next = malloc(sizeof(t_cmd)); // yeni cmd icin yer acmamiz gerektigi icin
    if (!(*cmd)->next)
        return 0;
    *token = (*token)->next; // pipe'dan sonraki token'a geciyoruz
    if(*token == NULL) // eger pipe'dan sonra token yoksa donguden cik
        return 0;
    *cmd = (*cmd)->next; // yeni cmd'ye geciyoruz
    fill_cmd(*cmd, *token); // yeni cmd'yi dolduruyoruzü
    return 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 14:16:13 by asay              #+#    #+#             */
/*   Updated: 2026/06/06 19:29:09 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *get_cmds (t_shell *sh) // parserda kaullanacagimiz icin tum cmd'leri almalıyım
{
    t_token *token;   // token listesinde dolasmamiz icin
    t_cmd   *cmd;     // olusturdugumuz cmd yapisi
    t_cmd   *head;         // dondurecegim degisken. cmd yapisinin basi.

    token = sh->tokens;
    cmd = malloc(sizeof(t_cmd)); // her cmd icin yer acmamiz gerektigi icin while'ın icinde
    if (!cmd)
        return NULL;
    fill_cmd(cmd, token); // cmd yapisini dolduruyoruz
    head = cmd; // cmd yapisinin basini kaybetmemek icin
    while (token != NULL) // token listesinde dolasarak pipe gorene kadar cmd'leri alacagiz
    {
        token = token->next;
        if(token == NULL)
            break;
        if (token->type == PIPE) // pipe gorunce cmd'yi bitirip yeni cmd'ye baslamamiz gerekiyor
            handle_pipe(&cmd, &token); // pipe gorunce cmd'yi bitirip yeni cmd'ye baslamamiz gerekiyor
    }
    cmd->next = NULL; // son cmd'nin next'ini NULL yaparak cmd listesinin sonunu belirtiyoruz.
    return (head);
}

char **get_argv(t_token *token)
{
    char **argv;
    int i;

    argv = malloc(sizeof(char *) * (word_count(token) + 1));
    i = 0;
    if (!argv)
        return NULL;
    while(token != NULL && token->type != PIPE)
    {
        if (token->type == WORD)
        {
            argv[i] = ft_strdup(token->context);
            i++;
        }
        if(token->type == REDIRECT_IN || token->type == REDIRECT_OUT || token->type == HEREDOC || token->type == APPEND)
        {
            if(token->next == NULL) // eger redirect'in targeti yoksa donguden cik
                break; 
            token = token->next; // redir sonrasi target word'u saymayalim diye
        }
        token = token->next;
    }
    argv[i] = NULL;
    return (argv);
}

t_redirect *get_redirs(t_token *token)
{
    t_redirect *rdr;
    t_redirect *head;
    t_redirect *pre;

    head = NULL;
    rdr = NULL;
    while (token != NULL && token->type != PIPE)
    {
        if (token->type == REDIRECT_IN || token->type == REDIRECT_OUT || token->type == HEREDOC || token->type == APPEND)
        {
            rdr = malloc(sizeof(t_redirect));
            if(!rdr)
                return NULL;
            rdr->type = token->type;
            if(head == NULL) // eger ilk redirect ise head'i guncelliyoruz
                head = rdr;
            else // eger ilk redirect degilse onceki redirectin nextine bagliyoruz
                pre->next = rdr; // yeni redirectin presi eski head oluyor
            pre = rdr; // yeni redirectin presi eski head oluyor
            rdr->target = ft_strdup(token->next->context);
            rdr->next = NULL; // son redirectin next'ini NULL yaparak redirect listesinin sonunu belirtiyoruz.
        }
        token = token->next;
    }
    return (head);
}

void parser (t_shell *sh)
{
    sh->cmds = get_cmds(sh);
}

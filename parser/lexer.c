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

// boşluk görüldüğünde bufferdaki veriyi listeye ekleyip buffer'ı boşaltır. sonra bir sonraki anlamlı ifadeye kadar boşlukları atlar.
void whitespace_tkn(t_lexer *ptr, char *str) 
{
    if(ptr->buff[0])
    {
        ptr->curr = new_token(ptr->value, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ptr->buff[0] = '\0';
        ptr->j = 0;   
    }
    while(str[ptr->i] == 32)
        ptr->i++;
}


void redirect_tkn(t_lexer *ptr, char *str)
{
    // eğer bufferde herhangi bir şey varsa ve boşluksuz yazılmışsa whitespace_tkn çağırılmaz.
    // bu yüzden whitesapce_tkn'nin ekleyemeyeceği WORD token'ları için bir kontrol ekledik.
    if(ptr->buff[0])
    {
        ptr->curr = new_token(WORD, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ptr->buff[0] = '\0';
        ptr->j = 0;   
    }
    //redirect > görüldüğü her koşulda eklenecek o yüzden herhangi bir if yok
    ptr->curr = new_token(ptr->value, ""); //zaten get_value'dan value burada saklanıyor. context'in > olduğu belli. direkt boş string veriyoruz. 
    add_token(&ptr->head, ptr->curr);
    while(str[ptr->i] == 32)
        ptr->i++;
}

void quote_tkn(t_lexer *ptr, char *str)
{
    if(str[ptr->i] == 34)
        ptr->in_double = 1;
    if(str[ptr->i] == 39)
        ptr->in_single = 1;
    ptr->i++;
    while(str[ptr->i] && (ptr->in_double || ptr->in_single))
    {
        if(str[ptr->i] == 34 && ptr->in_double)
        {
            ptr->in_double = 0;
            ptr->i++;
        }
        else if(str[ptr->i] == 39 && ptr->in_single)
        {            
            ptr->in_single = 0;
            ptr->i++;
        }
        else
        {
            ptr->buff[ptr->j] = str[ptr->i];
            ptr->i++;
            ptr->j++;
        }
    }
    ptr->curr = new_token(WORD, ptr->buff);
    add_token(&ptr->head, ptr->curr);
    ptr->buff[0] = '\0';
    ptr->j = 0;   
    while(str[ptr->i] == 32)
        ptr->i++;
}

void pipe_tkn(t_lexer *ptr, char *str)
{
    //yapılan işlemler redirect_tkn ile aynı. çağırıldıkları yerde karmaşa olmaması için şimdilik ayırdım.
    // eğer bufferde herhangi bir şey varsa ve boşluksuz yazılmışsa whitespace_tkn çağırılmaz.
    // bu yüzden whitesapce_tkn'nin ekleyemeyeceği WORD token'ları için bir kontrol ekledik.
    if(ptr->buff[0])
    {
        ptr->curr = new_token(WORD, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ptr->buff[0] = '\0';
        ptr->j = 0;   
    }
    //pipe | görüldüğü her koşulda eklenecek o yüzden herhangi bir if yok
    ptr->curr = new_token(ptr->value, ""); //zaten get_value'dan value burada saklanıyor. context'in | olduğu belli. direkt boş string veriyoruz. 
    add_token(&ptr->head, ptr->curr);
    while(str[ptr->i] == 32)
        ptr->i++;
}

t_token *get_tokens(char *str)
{
    t_lexer *ptr;

    ptr = malloc(sizeof(t_lexer));
    if(!ptr)
        return NULL;
    if(lexer_init(ptr, str) == -1)
        return NULL;
    while(str[ptr->i])
    {
        ptr->value = get_value(str, &(ptr->i));
        if(str[ptr->i] == 32)
            whitespace_tkn(ptr, str);
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


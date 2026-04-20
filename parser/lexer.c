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
    t_token *temp = tokens;
    while(temp)
    {
        printf("Type: %d, Context: '%s'\n", temp->type, temp->context);
        temp = temp->next;
    }
    //test bitti.
    return (1);
}

// boşluk görüldüğünde bufferdaki veriyi listeye ekleyip buffer'ı boşaltır. sonra bir sonraki anlamlı ifadeye kadar boşlukları atlar.
void whitespace_tkn(t_lexer *ptr, char *str) 
{
    if(ptr->buff[0])
    {
        ptr->curr = new_token(WORD, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ft_memset(ptr->buff, 0, ptr->j);
        ptr->j = 0; 
    }
    while(str[ptr->i] == 32 && str[ptr->i])
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
        ft_memset(ptr->buff, 0, ptr->j);
        ptr->j = 0;  
    }
    //redirect > görüldüğü her koşulda eklenecek o yüzden herhangi bir if yok
    if(ptr->value == APPEND)
        ptr->curr = new_token(ptr->value, ">>");
    else if(ptr->value == HEREDOC)
        ptr->curr = new_token(ptr->value, "<<");
    else if(ptr->value == REDIRECT_OUT)
        ptr->curr = new_token(ptr->value, ">");
    else
        ptr->curr = new_token(ptr->value, "<");
    add_token(&ptr->head, ptr->curr);
    if(ptr->value == APPEND || ptr->value == HEREDOC)
        ptr->i++;
    ptr->i++;
    while(str[ptr->i] == 32)
        ptr->i++;
    ft_memset(ptr->buff, 0, ptr->j);
    ptr->j = 0;
}

// çift tırnak veya tek tırnak görünce çağrılır ve tırnak karakterini atlayarak başlar, aynı tırnak karakteriyle karşılaşana kadar buffera yazmaya devam eder.
void quote_tkn(t_lexer *ptr, char *str)
{
    char quote;

    quote = str[ptr->i];
    ptr->i++;
    while(str[ptr->i] && str[ptr->i] != quote)
        ptr->buff[ptr->j++] = str[ptr->i++];
    ptr->i++; // quote karakterini atla
}
// "hello"aleyna"world" -> "helloaleynaworld" şeklinde tek bir token olarak alınacak. 
// yukarıdaki farklı kombinasyonlara göre fonksiyon geliştirilecek.
// bash nasıl çalışıyor inceleyip ona göre davranırız.

void pipe_tkn(t_lexer *ptr, char *str)
{
    //yapılan işlemler redirect_tkn ile aynı. çağırıldıkları yerde karmaşa olmaması için şimdilik ayırdım.
    // eğer bufferde herhangi bir şey varsa ve boşluksuz yazılmışsa whitespace_tkn çağırılmaz.
    // bu yüzden whitesapce_tkn'nin ekleyemeyeceği WORD token'ları için bir kontrol ekledik.
    if(ptr->buff[0])
    {
        ptr->curr = new_token(WORD, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ft_memset(ptr->buff, 0, ptr->j);
        ptr->j = 0;   
    }
    //pipe | görüldüğü her koşulda eklenecek o yüzden herhangi bir if yok
    ptr->curr = new_token(ptr->value, "|"); //zaten get_value'dan value burada saklanıyor. context'in | olduğu belli. direkt boş string veriyoruz. 
    add_token(&ptr->head, ptr->curr);
    ptr->i++;
    while(str[ptr->i] == 32)
        ptr->i++;
    ft_memset(ptr->buff, 0, ptr->j);
    ptr->j = 0;
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
    if (ptr->j > 0)
        add_token(&ptr->head, new_token(WORD, ptr->buff));

    return (ptr->head);
}


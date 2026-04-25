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
    shell->tokens = tokens;
    return (1);
}

// boşluk görüldüğünde bufferdaki veriyi listeye ekleyip buffer'ı boşaltır. sonra bir sonraki anlamlı ifadeye kadar boşlukları atlar.
void whitespace_tkn(t_lexer *ptr, char *str) 
{
    if(ptr->buff[0] || ptr->has_quote)
    {
        ptr->curr = new_token(WORD, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ft_memset(ptr->buff, 0, ptr->j);
        ptr->j = 0;
        ptr->has_quote = 0; 
    }
    while(str[ptr->i] == 32 && str[ptr->i])
        ptr->i++;
}

void redirect_tkn(t_lexer *ptr, char *str)
{
    // eğer bufferde herhangi bir şey varsa ve boşluksuz yazılmışsa whitespace_tkn çağırılmaz.
    // bu yüzden whitesapce_tkn'nin ekleyemeyeceği WORD token'ları için bir kontrol ekledik.
    if(ptr->buff[0] || ptr->has_quote)
    {
        ptr->curr = new_token(WORD, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ft_memset(ptr->buff, 0, ptr->j);
        ptr->j = 0;
        ptr->has_quote = 0;
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
    char quote_type;
    ptr->has_quote = 1; //' veya " gorgugumuzde has_quote'u 1 yapıyoruz ki kapanmamış tırnaklar haric her durumda (bos girdiler icin daha cok) get_tokens'taki if'e girilsin.
    //int temp;

    //temp = ptr->j;
    quote_type = str[ptr->i];
    ptr->i++;
    while(str[ptr->i] && str[ptr->i] != quote_type) //bir sonraki tırnaga kadar buffer dolduruyoruz.
        ptr->buff[ptr->j++] = str[ptr->i++];
    if (str[ptr->i] == quote_type) // tirnak kapandi 
        ptr->i++; // quote karakterini atla
    else
        printf("Error: Open quote!\n"); // quote kapanmadan fonksiyon bittiyse hata verdiriyoruz.
    //burada has_quote = 1 yapildigi icin get_tokens'a geri dondugumuzde sondaki if sayesidne "kullanici tirnak kullandi, bir WORD token'i eklenmeli" diyebilmeyi sagliyoruz
}
// "hello"aleyna"world" -> "helloaleynaworld" şeklinde tek bir token olarak alınacak. 
// yukarıdaki farklı kombinasyonlara göre fonksiyon geliştirilecek.
// bash nasıl çalışıyor inceleyip ona göre davranırız.

void pipe_tkn(t_lexer *ptr, char *str)
{
    //yapılan işlemler redirect_tkn ile aynı. çağırıldıkları yerde karmaşa olmaması için şimdilik ayırdım.
    // eğer bufferde herhangi bir şey varsa ve boşluksuz yazılmışsa whitespace_tkn çağırılmaz.
    // bu yüzden whitesapce_tkn'nin ekleyemeyeceği WORD token'ları için bir kontrol ekledik.
    if(ptr->buff[0] || ptr->has_quote)
    {
        ptr->curr = new_token(WORD, ptr->buff);
        add_token(&ptr->head, ptr->curr);
        ft_memset(ptr->buff, 0, ptr->j);
        ptr->j = 0;
        ptr->has_quote = 0;
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
    t_token *copy_of_head;

    ptr = malloc(sizeof(t_lexer));
    if(!ptr || lexer_init(ptr, str) == -1)
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
    if (ptr->j > 0 || ptr->has_quote) //eger has_quote quote_tkn sonrası 1 kalmıssa ve bufferde herhangi bir sey varsa """" gibi tokenlarda tek bir bos token ekleyebilsin
        add_token(&ptr->head, new_token(WORD, ptr->buff));
    copy_of_head = ptr->head;
    clean_get_tkns(ptr);
    return (copy_of_head);
}


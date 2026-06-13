/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <asay@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:38:00 by asay              #+#    #+#             */
/*   Updated: 2026/06/13 04:30:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

//kontrol edildi.
//gelen inputta str[*i]'nin ne olduğunu kontrol ediyoruz.
t_token_type	get_value(char *str, int *i) 
{
	if (str[*i] == '>')
	{
		if(str[*i+1] == '>')
			return (APPEND);
		return (REDIRECT_OUT);
	}
	else if (str[*i] == '<')
	{
		if(str[*i+1] == '<')
			return(HEREDOC);
		return (REDIRECT_IN);
	}
	else if(str[*i] == '|')
		return (PIPE);
	else
		return (WORD);
}

// boşluk görüldüğünde bufferdaki veriyi listeye ekleyip buffer'ı boşaltır. sonra bir sonraki anlamlı ifadeye kadar boşlukları atlar.
void whitespace_tkn(t_lexer *ptr, char *str) 
{
    if(ptr->buff[0] || ptr->has_quote)
    {
        ptr->curr = new_token(WORD, ptr->buff);
        is_gonna_expand(ptr, ptr->curr);
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
    if (ptr->buff[0] || ptr->has_quote)
    {
        ptr->curr = new_token(WORD, ptr->buff);
        is_gonna_expand(ptr, ptr->curr);
        add_token(&ptr->head, ptr->curr);
        ft_memset(ptr->buff, 0, ptr->j);
        ptr->j = 0;
        ptr->has_quote = 0;
    }
    //redirect > görüldüğü her koşulda eklenecek o yüzden herhangi bir if yok
    if (ptr->value == APPEND)
        ptr->curr = new_token(ptr->value, ">>");
    else if (ptr->value == HEREDOC)
        ptr->curr = new_token(ptr->value, "<<");
    else if (ptr->value == REDIRECT_OUT)
        ptr->curr = new_token(ptr->value, ">");
    else
        ptr->curr = new_token(ptr->value, "<");
    add_token(&ptr->head, ptr->curr);
    if (ptr->value == APPEND || ptr->value == HEREDOC)
        ptr->i++;
    ptr->i++;
    while (str[ptr->i] == 32)
        ptr->i++;
    ft_memset(ptr->buff, 0, ptr->j);
    ptr->j = 0;
}

// çift tırnak veya tek tırnak görünce çağrılır ve tırnak karakterini atlayarak başlar, aynı tırnak karakteriyle karşılaşana kadar buffera yazmaya devam eder.
void quote_tkn(t_lexer *ptr, char *str)
{
    char quote_type;
    ptr->has_quote = 1; //' veya " gorgugumuzde has_quote'u 1 yapıyoruz ki kapanmamış tırnaklar haric her durumda (bos girdiler icin daha cok) get_tokens'taki if'e girilsin.

    quote_type = str[ptr->i];
    if(quote_type == 34)
        ptr->in_double = 1;
    if(quote_type == 39)
        ptr->in_single = 1;
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
        is_gonna_expand(ptr, ptr->curr);
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

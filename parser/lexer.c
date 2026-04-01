/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:37:52 by asay              #+#    #+#             */
/*   Updated: 2026/04/01 19:04:22 by asay             ###   ########.fr       */
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
    return (1);
}

t_token *get_tokens(char *str)
{
    int i;
    int j;
    char *buff;
    t_token *head;
    t_token *curr;
    t_token_type value;

    i = 0;
    head = NULL;
    buff = malloc(sizeof(char) * (ft_strlen(str) + 1));
    if(!buff)
        return NULL;
    
    //get_value öncesi "$USER" -> asay : '$USER' -> $USER durumu kontrol edilmeli.
    while(str[i])
    {
        value = get_value(str, i);
        if(str[i] == 32)
        {
            if(!buff[0])
                i++;
            else
            {
                curr = new_token(value, buff);
                add_token(&head, curr);
                buff[0] = '\0';
                i++;
            }
        }
        else if(str[i] == '|' || str[i] == '<' || str[i] == '>')
        {
            if(!buff[0])
                i++;
            /* |, >, < durumu: new_token(type, value) çağırman lazım. 
                Tip belirleme mantığı da burada olacak — str[i+1]'e bakarak 
                PIPE, REDIRECT_OUT, APPEND vs. kararını veriyorsun.
            */
            else
            {    
                curr = new_token(value, buff);
                if(str)
                add_token(&head, curr);
                buff[0] = '\0';
                i++;
            }
        }
        else if(str[i] == 34 || str[i] == 39)
        {
            /*
            Her token tamamlandığında — boşluk, |/>/<, veya tırnak kapandığında — şu üç şeyi yapman lazım:

                - new_token ile token oluştur
                - add_token ile listeye ekle
                - buff[0] = '\0' ile buffer'ı sıfırla

            Normal karakter gördüğünde tek şey: buff[j++] = str[i] — karakteri buffer'a ekle, j'yi ilerlet.
            i her durumda ilerlemeli — bunu unutma, sonsuz döngü riski var.
            */
            if(!buff[0])
                i++;
            {    
                curr = new_token(value, buff);
                if(str)
                add_token(&head, curr);
                buff[0] = '\0';
                i++;
            }
        }
        else
            buff[j++] = str[i++];
    }
    return (head);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:05:32 by asay              #+#    #+#             */
/*   Updated: 2026/04/27 20:50:44 by asay             ###   ########.fr       */
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

//çağırdığın yerde free'lemeyi unutma!!
char *get_env_key(char *str,char *key, int i)
{
    int j;

    j = 0;
    key = malloc((ft_strlen(str) + 1));
    if(!key)
        return NULL;
    while(str[i] && (ft_isalnum(str[i]) || str[i] == '_')) //$ sonrasi sayi, harf ve '_' harici karakterler degisken adini bitiriyor.
    {
        key[j] = str[i];
        i++;
        j++; 
    }
    key[j] = '\0';
    if(!key)
        return (NULL);
    return (key);
}

char *get_env_value(t_shell *sh, char *key, char *value)
{
    int i;
    int j;
    int k;

    i = 0;
    j = ft_strlen(key) + 1;
    k = 0;
    while(sh->env[i])
    {
        if (ft_strncmp(sh->env[i], key, ft_strlen(key)) && sh->env[i][ft_strlen(key)] == '=')
        {
            value = malloc(ft_strlen(sh->env[i]) + 1);
            if(!value)
                return NULL;
            while(sh->env[i][j] || sh->env[i][j] != '\n')
                value[k++] = sh->env[i][j++];
        }
        i++;
    }
    value[k] = '\0';
    return (value);
}

void expander(t_shell *sh)
{
    int i;
    char *str;
    char *key;
    char *value;
    t_token  *curr_token;

    curr_token = sh->tokens;
    while(curr_token)
    {
        i = 0;
        str = curr_token->context;
        while(str[i])
        {
            if(curr_token->expand == 1 && str[i] == '$')
            {
                i++;
                get_env_key(str, key, i);
                get_env_value(sh, key, value);
                //gelen value değerini token context'inde key ile değiştirmem gerek. 
                free(key);
                free(value);
            }
            i++;
        }
        curr_token = curr_token->next;
    }
}

// expand fonksiyonlarini kontrol et, value değiştirme fonk.u sonrasında expand testleri yap.
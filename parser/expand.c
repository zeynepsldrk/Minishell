/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 17:05:32 by asay              #+#    #+#             */
/*   Updated: 2026/04/29 22:00:15 by asay             ###   ########.fr       */
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
char *get_env_key(char *str, int i)
{
    int j;
    char *key;

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

char *get_env_value(t_shell *sh, char *key)
{
    int i;
    int j;
    int k;
    char *value;

    i = 0;
    j = ft_strlen(key) + 1;
    k = 0;
    value = NULL;
    while(sh->env[i])
    {
        if (ft_strncmp(sh->env[i], key, ft_strlen(key)) == 0 && sh->env[i][ft_strlen(key)] == '=')
        {
            value = malloc(ft_strlen(sh->env[i]) + 1);
            if(!value)
                return NULL;
            while(sh->env[i][j] != '\0')
                value[k++] = sh->env[i][j++];
            break ; //kopyalma sonrasi en icinde gezmeyi birakiyroum.
        }
        i++;
    }
    if(value)
        value[k] = '\0'; //cagirdigin yerde "eger null döndüyse" kontrolü yapmayı unutma!!
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
                key = get_env_key(str, i);
                value = get_env_value(sh, key);
                if(!key || !value)
                    return ;
                ch_value();
                //gelen value değerini token context'inde key ile değiştirmem gerek. 
                free(key);
                free(value);
            }
            i++;
        }
        curr_token = curr_token->next;
    }
}

void ch_value(t_token *tkn)
{
    char *str;
    int i;

    str = tkn->context;
    i = 0;
    while(str[i] != '$')
        i++;
    
}


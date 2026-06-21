/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 15:24:47 by asay              #+#    #+#             */
/*   Updated: 2026/06/21 15:07:05 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    return (key);
}

char *get_env_value(t_shell *sh, char *key)
{
    char *value;

    value = my_little_getenv(sh->env_list, key);
    if (!value)
        return (NULL);
    return (ft_strdup(value));
}

char *ch_value(t_expander *exp)
{
    char *new_context;
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;
    if(exp->value)
        new_context = malloc(ft_strlen(exp->str) + ft_strlen(exp->value) + 1);
    else
        new_context = malloc(ft_strlen(exp->str) + 1);
    if(!new_context)
        return NULL;
    while(exp->str[i] != '$')
    {
        new_context[j++] = exp->str[i];
        i++;
    }
    i++;
    if(exp->value != NULL)
    {
        while(exp->value[k])
            new_context[j++] = exp->value[k++];
    }
    i += ft_strlen(exp->key);
    while(exp->str[i])
        new_context[j++] = exp->str[i++];
    new_context[j] = '\0';
    return new_context;
}
// str:         hello $USER/docs 
// new_context: hello asay/docs
// hello asay/docs

void rm_empty_token(t_shell *sh)
{
    t_token *curr;
    t_token *prev;
    t_token *tmp;
    
    curr = sh->tokens;
    prev = NULL;
    while(curr != NULL)
    {
        //bash tek basina olan boslari silmiyo bosluk alıyo o yuzden is_joined1se siliyoruz
        if(curr->context && curr->context[0] == '\0' && curr->is_joined) 
        {
            tmp = curr->next;
            if(prev == NULL) //silinecek token head ise
                sh->tokens = tmp;
            else
                prev->next = tmp;
            free(curr->context);
            free(curr);
            curr = tmp; 
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

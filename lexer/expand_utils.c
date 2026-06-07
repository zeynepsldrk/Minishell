/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 15:24:47 by asay              #+#    #+#             */
/*   Updated: 2026/06/06 19:29:44 by asay             ###   ########.fr       */
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
     printf("DEBUG get_env_key -> str: '%s', i_start: %d, key: '%s'\n", str, i - j, key); // geçici
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
            break ; //kopyalma sonrasi env icinde gezmeyi birakiyroum.
        }
        i++;
    }
    if(value)
        value[k] = '\0'; //cagirdigin yerde "eger null döndüyse" kontrolü yapmayı unutma!!
    return (value);
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
    printf("str: '%s', key: '%s'\n", exp->str, exp->key);
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

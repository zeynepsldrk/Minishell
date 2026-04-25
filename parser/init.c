/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 21:02:35 by asay              #+#    #+#             */
/*   Updated: 2026/04/02 21:33:08 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **copy_env(char **env)
{
    char **dupe_env;
    int i;

    i = 0;
    while(env[i])
        i++;
    dupe_env = malloc(sizeof(char *) * (i + 1));
    if(!dupe_env)
        return NULL;
    i = 0;
    while(env[i])
    {
        dupe_env[i] = ft_strdup(env[i]);
        if(!dupe_env[i])
        {
            free_str(dupe_env);
            return (NULL);
        }
        i++;
    }
    dupe_env[i] = NULL;
    return (dupe_env);
}

int sh_init(t_shell *shell, char **env)
{
    shell->cmds = NULL;
    shell->input = NULL;
    shell->tokens = NULL;
    shell->exit_value = 0; //bash ilk çalıştığında exit değeri için echo $? denildiğinde 0 dönüyor. 
    shell->env = copy_env(env);
    if(!shell->env)
        return (-1);
    return (0);   
}

int lexer_init(t_lexer *ptr, char *str)
{
    int i;

    i = 0;
    ptr->buff = malloc(sizeof(char) * (ft_strlen(str) + 1));
    if(!ptr->buff)
        return (-1);
    ptr->head = NULL;
    ptr->curr = NULL;
    ptr->i = 0;
    ptr->j = 0;
    while(i < (ft_strlen(str) + 1))
        ptr->buff[i++] = 0;
    ptr->in_single = 0;
    ptr->in_double = 0;
    ptr->has_quote = 0;
    return (0);
}

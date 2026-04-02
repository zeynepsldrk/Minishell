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

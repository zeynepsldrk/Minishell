/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:03:29 by asay              #+#    #+#             */
/*   Updated: 2026/04/01 20:35:32 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int prompt(t_shell *shell) //readline basmalı
{
    char *line;
    char *trimmed;

    line = readline("minishell > ");
    if(!line)
    {
        write(1, "\n", 1);
        return (0);
    }
    if (line[0] == '\0')
    {
        free(line);
        return (0);
    }
    add_history(line);
    trimmed = trim(line);
    free(line);
    if(!trimmed || trimmed[0] == '\0')
    {
        free(trimmed);
        return 0;
    }
    (void)shell;
    //lexer(shell, trimmed);
    free(trimmed);
    return 0;
}

int main(int argc, char **argv, char **env)
{
    t_shell *shell;
    (void)argv;
    (void)argc;
    (void)env;
    
    shell = NULL;
    // init func.
    //env = getenv()
    while(prompt(shell))
    {
        
        
    }
    //cleaning
    return(shell->exit_value);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:03:29 by asay              #+#    #+#             */
/*   Updated: 2026/04/02 21:43:15 by asay             ###   ########.fr       */
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
        return (1);
    }
    add_history(line);
    trimmed = trim(line);
    free(line);
    if(!trimmed || trimmed[0] == '\0')
    {
        free(trimmed);
        return (1);
    }
    (void)shell;
    //lexer(shell, trimmed);
    free(trimmed);
    return (1);
}

int main(int argc, char **argv, char **env)
{
    t_shell *shell;
    (void)argv;
    (void)argc;

    shell = malloc(sizeof(t_shell));
    if (!shell)
        return 1; 
    if (sh_init(shell, env) == -1)
    {
        write(2, "Initialization failed\n", 22);
        return (1);
    }
    while(prompt(shell))
    {
        
    }
    free_sh(shell);
    return(shell->exit_value);
}

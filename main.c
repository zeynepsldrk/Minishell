/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asay <asay@student.42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:03:29 by asay              #+#    #+#             */
/*   Updated: 2026/06/06 19:29:28 by asay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int prompt(t_shell *shell) //readline basmalı
{
    char *line;
    char *trimmed;

    line = readline("minishell > ");
    if(!line)
        return (write(1, "\n", 1), 0);
    if (line[0] == '\0')
        return (free(line), 1);
    add_history(line);
    trimmed = trim(line);
    free(line);
    if(!trimmed || trimmed[0] == '\0')
    {
        free(trimmed);
        return (1);
    }
    lexer(shell, trimmed);
    expander(shell);
    //test basladi
    t_token *temp = shell->tokens;
    while(temp)
    {
        printf("Type: %d, Context: '%s'\n", temp->type, temp->context);
        printf("Expand Situation: %d\n", temp->expand);
        temp = temp->next;
    }	
    // test bitti
    free_tokens(shell->tokens);
    shell->tokens = NULL;
    free(trimmed);
    return (1);
}

int main(int argc, char **argv, char **env)
{
    t_shell *shell;
    int exit;
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
    exit = shell->exit_value;
    free_sh(shell);
    return(exit);
}

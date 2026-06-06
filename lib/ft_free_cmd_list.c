#include "minishell.h"

static void free_array(char **arr)
{
    int i;

    if (!arr)
        return ;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void ft_free_cmd_list(t_cmd *cmds)
{
    t_cmd *current;
    t_cmd *next;

    current = cmds;
    while (current)
    {
        next = current->next;
        if (current->args)
            free_array(current->args);   // char **args -> her elemanı free
        if (current->input_file)
            free(current->input_file);
        if (current->output_file)
            free(current->output_file);
        free(current);
        current = next;
    }
}

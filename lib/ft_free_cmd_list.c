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
    t_redirect *redir;
    t_redirect *redir_next;

    current = cmds;
    while (current)
    {
        next = current->next;
        if (current->argv)
            free_array(current->argv);
        redir = current->redirects;
        while (redir)
        {
            redir_next = redir->next;
            if (redir->target_file)
                free(redir->target_file);
            free(redir);
            redir = redir_next;
        }
        free(current);
        current = next;
    }
}

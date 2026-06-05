#include "minishell.h"

static void free_env_matrix(char **matrix)
{
    int i;

    if (!matrix)
        return ;
    i = 0;
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}

static int  count_env_nodes(t_env_node *list)
{
    int count;

    count = 0;
    while (list)
    {
        count++;
        list = list->next;
    }
    return (count);
}

static char *join_env_str(char *key, char *value)
{
    char    *temp;
    char    *res;

    if (!value)
        return (ft_strdup(key));
    temp = ft_strjoin(key, "=");
    res = ft_strjoin(temp, value);
    free(temp);
    return (res);
}

void    ft_check_env(t_shell *shell)
{
    int         i;
    int         count;
    t_env_node  *temp;

    if (shell->env)
        free_env_matrix(shell->env);
    count = count_env_nodes(shell->env_list);
    shell->env = (char **)malloc(sizeof(char *) * (count + 1));
    if (!shell->env)
        return ;
    i = 0;
    temp = shell->env_list;
    while (temp)
    {
        shell->env[i] = join_env_str(temp->key, temp->value);
        i++;
        temp = temp->next;
    }
    shell->env[i] = NULL;
}

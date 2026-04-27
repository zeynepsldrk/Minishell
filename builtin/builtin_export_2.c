#include "minishell.h"

int is_key_inside(char *key, t_env_node *env_list)
{
    while (env_list)
    {
        if (ft_strcmp(env_list->key, key) == 0)
            return (1);
        env_list = env_list->next;
    }
    return (0);
}

char *find_key_or_value(char *arg, int i, int which_one)
{
    char    *result;
    int     len;

    if (which_one == KEY)
    {
        len = 0;
        while (arg[len] && arg[len] != '=' && !(arg[len] == '+' && arg[len + 1] == '='))
            len++;
        result = ft_substr(arg, 0, len);
        return (result);
    }
    if (which_one == VALUE)
    {
        while (arg[i])
        {
            if (arg[i] == '+' && arg[i + 1] == '=')
                return (ft_strdup(arg + i + 2));
            if (arg[i] == '=')
                return (ft_strdup(arg + i + 1));
            i++;
        }
    }
    return (NULL);
}

#include "minishell.h"

int is_valid_identifier(char *arg)
{
    int j;

    j = 0;
    if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
        return (1);
    while (arg[j] && arg[j] != '=')
    {
        if (arg[j] == '+' && arg[j + 1] == '=')
            return (0);
        if (!(ft_isalnum(arg[j]) || arg[j] == '_'))
            return (1);
        j++;
    }
    return (0);
}

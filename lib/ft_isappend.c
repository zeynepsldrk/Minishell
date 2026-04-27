#include "minishell.h"

int  ft_isappend(char *arg)
{
    int i;

    i = 0;
    while (arg[i] && arg[i] != '=')
    {
        if (arg[i] == '+' && arg[i + 1] == '=')
            return (1);
        i++;
    }
    return (0);
}
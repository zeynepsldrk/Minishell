#include "minishell.h"

#include <stddef.h>

void    ft_strlcpy(char *dst, const char *src, size_t size)
{
    size_t  i;

    if (size == 0)
        return ;
    i = 0;
    while (src[i] && i < (size - 1))
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

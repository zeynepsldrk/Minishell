#include "minishell.h"

#include <stddef.h>

void    ft_strlcat(char *dst, const char *src, size_t size)
{
    size_t  i;
    size_t  dst_len;

    dst_len = 0;
    while (dst[dst_len] && dst_len < size)
        dst_len++;
    if (size <= dst_len)
        return ;
    i = 0;
    while (src[i] && (dst_len + i) < (size - 1))
    {
        dst[dst_len + i] = src[i];
        i++;
    }
    dst[dst_len + i] = '\0';
}

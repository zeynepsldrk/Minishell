#include "minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*subs;
	size_t	size;

	if (!s)
	{
		return (NULL);
	}
	if (start >= ft_strlen(s))
	{
		return (ft_strdup(""));
	}
	size = ft_strlen(s + start);
	if (size < len)
	{
		len = size;
	}
	subs = (char *)malloc(sizeof(char) * (len + 1));
	if (!subs)
	{
		return (NULL);
	}
	ft_strlcpy(subs, s + start, len + 1);
	return (subs);
}
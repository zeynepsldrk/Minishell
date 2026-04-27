#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	s1len;
	size_t	s2len;

	if (!s1 || !s2)
	{
		return (NULL);
	}
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	newstr = (char *)malloc((s1len + s2len + 1) * sizeof(char));
	if (!newstr)
	{
		return (NULL);
	}
	ft_strlcpy(newstr, s1, s1len + 1);
	ft_strlcat(newstr, s2, s1len + s2len + 1);
	return (newstr);
}
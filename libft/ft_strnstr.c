/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen < xingchen@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:04:14 by xingchen          #+#    #+#             */
/*   Updated: 2025/11/19 21:42:45 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!s1 && len == 0)
		return (NULL);
	if (s2[0] == '\0')
		return ((char *)s1);
	while (s1[i] && i < len)
	{
		j = 0;
		while (i + j < len && s1[i + j] == s2[j] && s1[i + j] && s2[j])
			j ++;
		if (s2[j] == '\0')
			return ((char *)&s1[i]);
		i ++;
	}
	return (NULL);
}
/*#include <stdio.h>
#include <string.h>
int main(void)
{
	char str1[] = "aahell00";
	char str2[] = "he";

	printf(" %s",strnstr(str1, str2, 0));

}*/

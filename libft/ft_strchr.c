/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:16:47 by xingchen          #+#    #+#             */
/*   Updated: 2025/11/14 18:59:34 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*s1;
	size_t	i;

	s1 = (char *)s;
	i = 0;
	while (i <= ft_strlen(s1))
	{
		if ((s1[i]) == (char)c)
			return (s1 + i);
		i ++;
	}
	return (NULL);
}
/*#include <stdio.h>
int main(void)
{
	char s[] = "hello";
	int	a = 0;
	printf("%s",ft_strchr(s,a));
}*/
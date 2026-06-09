/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 11:44:15 by xingchen          #+#    #+#             */
/*   Updated: 2026/04/21 18:02:54 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*fill_dest(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n - 1 && src[i])
	{
		dest[i] = src[i];
		i ++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	src_len;

	src_len = ft_strlen(src);
	if (!dest && size == 0)
		return (src_len);
	if (size == 0)
		return (src_len);
	else
		fill_dest(dest, src, size);
	return (src_len);
}
/*#include <stdio.h>
#include <string.h>
int main(void)
{
	char src[] = "hell0";
	char dest[20];
	size_t i;
	//i = strlcat(NULL,src,0);
	//printf("%zu", i);
	i = strlcat(NULL,src,1);
	printf("%zu", i);
}*/

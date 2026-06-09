/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen < xingchen@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 12:34:32 by xingchen          #+#    #+#             */
/*   Updated: 2025/11/19 21:41:02 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	len;
	size_t	destlen;

	if (!dest)
		destlen = 0;
	else
		destlen = ft_strlen(dest);
	len = destlen + ft_strlen(src);
	if (size < destlen + 1)
		return (size + ft_strlen(src));
	i = 0;
	while (i < size - destlen - 1 && src[i])
	{
		dest[destlen + i] = src[i];
		i ++;
	}
	dest[destlen + i] = '\0';
	return (len);
}
/*#include <stdio.h>
#include <string.h>
int main(void)
{
	char src[] = "world";
	//char dest[20] = "a";
	size_t i;

	i = ft_strlcat(NULL,src,0);
	printf("长度为 %zu",i);
	//i = strlcat(NULL,src,0);
	//printf("dest长度为%zu",i);

}*/
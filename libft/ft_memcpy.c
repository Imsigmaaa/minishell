/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen < xingchen@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:41:23 by xingchen          #+#    #+#             */
/*   Updated: 2025/11/19 18:06:26 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!src && !dest && n != 0)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i ++;
	}
	return (dest);
}
/*#include <stdio.h>
#include <string.h>
int main(void)
{
	//char str[] = "123456";
	char *string;
	string = ft_memcpy(NULL,NULL,3);
	//string = memcpy(NULL,NULL,3);
	printf("%s", string);
	
	
	return (0);
}*/
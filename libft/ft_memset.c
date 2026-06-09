/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen < xingchen@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 19:09:08 by xingchen          #+#    #+#             */
/*   Updated: 2025/11/19 21:41:21 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		p[i] = (unsigned char)c;
		i ++;
	}
	return (p);
}
/*#include <stdio.h>
#include <string.h>
int main(void)
{
	//char c[] = "hewaw";
	//ft_memset(c, 128, 2);
	//printf(" %p",ft_memset(NULL, 128, 2));
	//memset(NULL,128,2);
	printf(" %p",memset(NULL,128,2));
}*/
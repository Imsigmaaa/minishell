/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 15:50:07 by xingchen          #+#    #+#             */
/*   Updated: 2025/11/14 19:08:15 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i ++;
	}
}
/*#include <stdio.h>
int main(void)
{
	char s[]="hello";
	ft_bzero(s,2);
	int i = 0;
	while(i < 5)
	{
		printf("%c",s[i]);
		i ++;
	}
}*/
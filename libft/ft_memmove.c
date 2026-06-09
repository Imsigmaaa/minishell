/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen < xingchen@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:20:10 by xingchen          #+#    #+#             */
/*   Updated: 2025/11/19 18:24:57 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (!d && !s && n != 0)
		return (NULL);
	if (n == 0)
		return (d);
	if (d > s)
	{
		while (n > 0)
		{
			d[n - 1] = s[n - 1];
			n --;
		}
	}
	else
		ft_memcpy(d, s, n);
	return (d);
}

/*#include <stdio.h>
#include <string.h>
int main(void) 
{
   
    char str[] = "0123456";
	char *string1;
	char *string2;
	//string1 = ft_memmove(str + 3,str + 3,0);
	//printf("string1= %s\nstr= %s", string1, str);
	string2 = memmove(str + 3,str + 4,0);
	printf("string2= %s\nstr= %s", string2, str);
	//printf("%s", str);
		
	return (0);
}*/

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 14:59:38 by xingchen          #+#    #+#             */
/*   Updated: 2026/04/21 18:01:24 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	unsigned int	tran_n(int n)
{
	unsigned int	num;

	if (n < 0)
		num = - (unsigned int)n;
	else
		num = (unsigned int)n;
	return (num);
}

static unsigned int	count_words(int n)
{
	unsigned int	i;
	unsigned int	num;

	i = 1;
	num = tran_n(n);
	while (num > 9)
	{
		num = num / 10;
		i ++;
	}
	if (n < 0)
		return (i + 1);
	return (i);
}

static char	*cpy_word(char *dup, unsigned int num, unsigned int index, int n)
{
	char	c;

	dup[index] = '\0';
	index --;
	while (num > 9)
	{
		c = num % 10 + '0';
		dup[index] = c;
		num = num / 10;
		index --;
	}
	c = num + '0';
	dup[index] = c;
	if (n < 0)
		dup[0] = '-';
	return (dup);
}

char	*ft_itoa(int n)
{
	unsigned int	index;
	unsigned int	num;
	char			*dup;

	index = count_words(n);
	num = tran_n(n);
	dup = malloc(index + 1);
	if (!dup)
		return (NULL);
	cpy_word(dup, num, index, n);
	return (dup);
}

/*int main(void)
{
	int	n = -2147483648;
	unsigned int num = tran_n(n);
	unsigned int count = count_words(n);

	char *s = ft_itoa(n);
	printf("%s\n%d\n%d\n",s,num,count);
	free(s);

}*/
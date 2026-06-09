/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:16:18 by xingchen          #+#    #+#             */
/*   Updated: 2026/04/21 18:03:26 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	start_index(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!set)
		return (i);
	while (s1[i])
	{
		j = 0;
		while (set[j] && s1[i] != set[j])
			j ++;
		if (s1[i] != set[j])
			return (i);
		i ++;
	}
	return (i);
}

static	size_t	end_index(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	s1_len;

	if (ft_strlen(s1) == 0)
		return (0);
	else
		s1_len = ft_strlen(s1);
	i = s1_len - 1;
	if (!set)
		return (i);
	while (i > 0)
	{
		j = 0;
		while (set[j] && s1[i] != set[j])
			j ++;
		if (s1[i] != set[j])
			return (i);
		i --;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*dup;
	size_t	size;
	size_t	start;
	size_t	end;

	if (!s1)
		return (NULL);
	start = start_index(s1, set);
	end = end_index(s1, set);
	if (start > end || !s1[0])
		size = 1;
	else
		size = end - start + 2;
	dup = malloc(size);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s1 + start, size - 1);
	dup[size - 1] = '\0';
	return (dup);
}
/*#include <stdio.h>

int	main(void)
{
	char const s1[] = "agg123";
	char const set[] = "+";
	char *dup;
	int i = 0;

	dup = ft_strtrim(s1,set);
	while(i < 10)
	{
		printf("%c",dup[i]);
		i ++;
	}
	free(dup);
}*/

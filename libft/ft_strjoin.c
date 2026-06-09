/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:33:21 by xingchen          #+#    #+#             */
/*   Updated: 2026/03/20 12:58:18 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s2_len;
	size_t	s1_len;
	char	*dup;

	if (!s1)
		s1_len = 0;
	else
		s1_len = ft_strlen(s1);
	if (!s2)
		s2_len = 0;
	else
		s2_len = ft_strlen(s2);
	dup = malloc(s1_len + s2_len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s1, s1_len);
	ft_memcpy(dup + s1_len, s2, s2_len);
	dup[s1_len + s2_len] = '\0';
	return (dup);
}
/*#include <stdio.h>

int	main(void)
{
	char const s1[] = "";
	char const s2[] = "";
	char *dup;
	int	i = 0;

	dup = ft_strjoin(s1,s2);
	while (i < 11)
	{
		printf("%c",dup[i]);
		i ++;
	}
	free(dup);
}*/
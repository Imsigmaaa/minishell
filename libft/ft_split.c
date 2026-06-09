/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:18:54 by xingchen          #+#    #+#             */
/*   Updated: 2026/03/20 12:58:06 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i ++;
		if (s[i] && s[i] != c)
		{
			count ++;
			i ++;
			while (s[i] && s[i] != c)
				i ++;
		}
	}
	return (count);
}

static char	*cpy_words(char const *s, char c, size_t *dup_j_len)
{
	size_t	j;
	char	*temp;

	while (s[*dup_j_len] && s[*dup_j_len] != c)
		(*dup_j_len)++;
	temp = malloc(*dup_j_len + 1);
	if (!temp)
		return (NULL);
	j = 0;
	while (j < *dup_j_len)
	{
		temp[j] = s[j];
		j ++;
	}
	temp[j] = '\0';
	return (temp);
}

static	char	*fill_array(char **dup, char const *s, char c, size_t *index)
{
	size_t	dup_i_len;

	*dup = NULL;
	while (s[*index] && s[*index] == c)
		(*index)++;
	dup_i_len = 0;
	if (s[*index] && s[*index] != c)
	{
		(*dup) = cpy_words(s + *index, c, &dup_i_len);
		if (!(*dup))
			return (NULL);
	}
	*index = *index + dup_i_len;
	return (*dup);
}

char	**ft_split(char const *s, char c)
{
	char	**dup;
	size_t	dup_len;
	size_t	index;
	size_t	j;

	dup_len = count_words(s, c);
	dup = (char **)malloc(sizeof(char *) * (dup_len + 1));
	if (!dup)
		return (NULL);
	index = 0;
	j = 0;
	while (s[index] && j < dup_len)
	{
		fill_array(&dup[j], s, c, &index);
		if (!dup[j])
		{
			while (j > 0)
				free(dup [--j]);
			free(dup);
			return (NULL);
		}
		j ++;
	}
	dup[dup_len] = NULL;
	return (dup);
}
/*int	main(void)
{
	char *s = "olol                     ";
    char **result = ft_split(s, ' ');

	int i = 0;
	while(result[i])
	{
		printf("%s\n",result[i]);
		i ++;
	}
	i = 0;
	while(result[i])
	{
		free(result[i]);
		i ++;
	}
	free(result);
	return (0);
}*/
/*char const *find_start(char const *s, char c)
{
	size_t	i;
	
	i = 0;
	while(s[i] && s[i] == c)
		i ++;
	return (s + i);
}
char const *find_end(char const *s, char c)
{
	size_t	i;
	
	i = 0;
	while(s[i] && s[i] != c)
		i ++;
	return (s + i);
}
char	**fill_dup_array(char **dup, char const *s, char c)
{
	size_t len;
	size_t j;
	size_t i;
	size_t k;
	char	*start;
	char	*end;

	i = 0;
	k = 0;
	while (s[i])
	{
		start = (char *)find_start(s + i, c);
		i = start - s;
		while (s[i] && s[i] == c)
    		i++;
		end = (char *)find_end(s + i, c);
		i = end - s;
		len = end - start;
		dup[k] = malloc(len + 1);
		if(!dup[k])
			return (NULL);
		j = 0;
		while(start[j] != c && start[j])
		{
			dup[k][j] = start[j];
			j ++;
		}
		dup[k][j] = '\0';
		k ++;
	}
	dup[k] = NULL;
	return (NULL);
}*/
/*if (!dup[j])
		{
    		while (j > 0)
        		free(dup[--j]);
    			free(dup);
    			return;
		}*/
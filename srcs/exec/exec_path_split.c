/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:48:44 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/16 21:48:41 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_path_parts(char *path)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 1;
	while (path[i])
	{
		if (path[i] == ':')
			count ++;
		i ++;
	}
	return (count);
}

static char	*cpy_words(char *path, size_t *one_path_len)
{
	size_t	i;
	char	*tmp;

	while (path[*one_path_len] && path[*one_path_len] != ':')
		(*one_path_len)++;
	tmp = malloc(*one_path_len + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < *one_path_len)
	{
		tmp[i] = path[i];
		i ++;
	}
	tmp[i] = '\0';
	return (tmp);
}

static	int	is_empty_path(char *path, size_t *index)
{
	size_t	path_len;

	path_len = ft_strlen(path);
	if ((*index == 0 && path[0] == ':')
		|| (path_len == 0)
		|| (*index == path_len - 1 && path[path_len - 1] == ':')
		|| (*index == path_len && path[path_len - 1] == ':')
		|| (*index + 1 < path_len && path[*index] == ':'
			&& path[*index + 1] == ':'))
		return (1);
	return (0);
}

static	char	*fill_array(char **one_path, char *path, size_t *index)
{
	size_t	one_path_len;

	*one_path = NULL;
	if (is_empty_path(path, index))
	{
		(*index)++;
		*one_path = ft_strdup(".");
		return (*one_path);
	}
	while (path[*index] && path[*index] == ':')
		(*index)++;
	one_path_len = 0;
	if (path[*index])
	{
		(*one_path) = cpy_words(path + *index, &one_path_len);
		if (!(*one_path))
			return (NULL);
	}
	*index = *index + one_path_len;
	return (*one_path);
}

char	**split_exec_path(char *path)
{
	char	**all_paths;
	size_t	size;
	size_t	index;
	size_t	j;

	size = count_path_parts(path);
	all_paths = (char **)malloc(sizeof(char *) * (size + 1));
	if (!all_paths)
		return (NULL);
	index = 0;
	j = 0;
	while (j < size)
	{
		fill_array(&all_paths[j], path, &index);
		if (!all_paths[j])
		{
			all_paths[j] = NULL;
			return (env_free_array(all_paths), NULL);
		}
		j ++;
	}
	all_paths[size] = NULL;
	return (all_paths);
}

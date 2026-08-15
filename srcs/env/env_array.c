/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:55:57 by yushan            #+#    #+#             */
/*   Updated: 2026/06/14 23:56:24 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_export_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

void	env_free_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**env_to_array(t_env *env)
{
	char	**array;
	int		i;

	array = malloc(sizeof(char *) * (env_export_count(env) + 1));
	if (!array)
		return (NULL);
	i = 0;
	array[0] = NULL;
	while (env)
	{
		if (env->exported && env->value)
		{
			array[i] = env_join_pair(env->key, env->value);
			if (!array[i])
				return (env_free_array(array), NULL);
			i++;
			array[i] = NULL;
		}
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

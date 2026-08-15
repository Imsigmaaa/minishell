/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:53:51 by yushan            #+#    #+#             */
/*   Updated: 2026/06/14 23:54:24 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_find(t_env *env, char *key)
{
	while (env)
	{
		if (env_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*env_get(t_env *env, char *key)
{
	t_env	*node;

	node = env_find(env, key);
	if (!node)
		return (NULL);
	return (node->value);
}

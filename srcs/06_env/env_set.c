/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:54:31 by yushan            #+#    #+#             */
/*   Updated: 2026/06/14 23:55:13 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_update_value(t_env *node, char *value)
{
	char	*new_value;

	if (!value)
		return (1);
	new_value = ms_strdup(value);
	if (!new_value)
		return (0);
	free(node->value);
	node->value = new_value;
	return (1);
}

int	env_set(t_env **env, char *key, char *value, int exported)
{
	t_env	*node;
	t_env	*new_node;

	node = env_find(*env, key);
	if (node)
	{
		if (!env_update_value(node, value))
			return (0);
		if (exported)
			node->exported = 1;
		return (1);
	}
	new_node = env_new(key, value, exported);
	if (!new_node)
		return (0);
	env_add_back(env, new_node);
	return (1);
}

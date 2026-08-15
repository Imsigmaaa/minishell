/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:52:24 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 02:49:50 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_new(char *key, char *value, int exported)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
		return (free(node), NULL);
	node->value = NULL;
	if (value)
		node->value = ft_strdup(value);
	if (value && !node->value)
		return (free(node->key), free(node), NULL);
	node->exported = exported;
	node->next = NULL;
	return (node);
}

void	env_add_back(t_env **env, t_env *new_node)
{
	t_env	*cur;

	if (!*env)
	{
		*env = new_node;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}

void	env_free(t_env **env)
{
	t_env	*tmp;

	while (env && *env)
	{
		tmp = (*env)->next;
		free((*env)->key);
		free((*env)->value);
		free(*env);
		*env = tmp;
	}
}

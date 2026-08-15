/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 14:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	put_text(char *text)
{
	write(1, text, ms_strlen(text));
}

static t_env	*next_exported(t_env *env, char *last_key)
{
	t_env	*next;

	next = NULL;
	while (env)
	{
		if (env->exported && (!last_key
				|| env_strcmp(env->key, last_key) > 0)
			&& (!next || env_strcmp(env->key, next->key) < 0))
			next = env;
		env = env->next;
	}
	return (next);
}

static void	put_export_value(char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if (value[i] == '"' || value[i] == '\\')
			write(1, "\\", 1);
		write(1, value + i, 1);
		i++;
	}
}

static void	print_export_node(t_env *node)
{
	put_text("declare -x ");
	put_text(node->key);
	if (node->value)
	{
		put_text("=\"");
		put_export_value(node->value);
		put_text("\"");
	}
	put_text("\n");
}

void	print_export_sorted(t_env *env)
{
	t_env	*node;
	char	*last_key;

	last_key = NULL;
	node = next_exported(env, last_key);
	while (node)
	{
		print_export_node(node);
		last_key = node->key;
		node = next_exported(env, last_key);
	}
}

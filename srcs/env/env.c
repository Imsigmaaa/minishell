/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 00:00:00 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/28 01:43:51 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_key_equal(char *s1, char *s2)
{
	size_t	len;

	if (!s1 || !s2)
		return (0);
	len = ft_strlen(s1);
	if (len != ft_strlen(s2))
		return (0);
	return (ft_strncmp(s1, s2, len) == 0);
}

static t_env	*new_env_node(char *key, char *value)
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
	{
		node->value = ft_strdup(value);
		if (!node->value)
			return (free(node->key), free(node), NULL);
	}
	node->next = NULL;
	return (node);
}

void	free_env(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next;
	}
}

static void	append_env_node(t_env **env, t_env *new)
{
	t_env	*current;

	if (!*env)
	{
		*env = new;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new;
}

static t_env	*node_from_envp(char *entry)
{
	char	*equal;
	char	*key;
	t_env	*node;

	equal = ft_strchr(entry, '=');
	if (!equal)
		return (new_env_node(entry, NULL));
	key = ft_substr(entry, 0, equal - entry);
	if (!key)
		return (NULL);
	node = new_env_node(key, equal + 1);
	free(key);
	return (node);
}

t_env	*copy_environment(char **envp)
{
	t_env	*env;
	t_env	*new;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new = node_from_envp(envp[i]);
		if (!new)
		{
			free_env(env);
			return (NULL);
		}
		append_env_node(&env, new);
		i++;
	}
	return (env);
}

t_env	*find_env(t_env *env, char *key)
{
	while (env)
	{
		if (env_key_equal(env->key, key))
			return (env);
		env = env->next;
	}
	return (NULL);
}

/*
 * 返回链表内部保存的 value。
 * 调用者只能读取，不能 free。
 */
char	*get_env_value(t_env *env, char *key)
{
	t_env	*node;

	node = find_env(env, key);
	if (!node || !node->value)
		return (NULL);
	return (node->value);
}

/*
 * key 不存在：创建新节点。
 * key 已存在且 value 不为 NULL：修改 value。
 * key 已存在且 value 为 NULL：保持原值。
 *
 * value 为 NULL 可以用于：
 * export TEST
 *
 * value 不为 NULL 可以用于：
 * export TEST=hello
 */
int	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*node;
	char	*new_value;

	node = find_env(*env, key);
	if (!node)
	{
		node = new_env_node(key, value);
		if (!node)
			return (0);
		append_env_node(env, node);
		return (1);
	}
	if (!value)
		return (1);
	new_value = ft_strdup(value);
	if (!new_value)
		return (0);
	free(node->value);
	node->value = new_value;
	return (1);
}

int	unset_env(t_env **env, char *key)
{
	t_env	*current;
	t_env	*previous;

	current = *env;
	previous = NULL;
	while (current)
	{
		if (env_key_equal(current->key, key))
		{
			if (previous)
				previous->next = current->next;
			else
				*env = current->next;
			current->next = NULL;
			free_env(current);
			return (1);
		}
		previous = current;
		current = current->next;
	}
	return (1);
}

/*
 * 合法：
 * USER
 * TEST_1
 * _VALUE
 *
 * 不合法：
 * 1TEST
 * TEST-NAME
 * TEST!
 */
int	is_valid_env_key(char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	count_exported_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*env_node_to_string(t_env *env)
{
	char	*prefix;
	char	*result;

	prefix = ft_strjoin(env->key, "=");
	if (!prefix)
		return (NULL);
	result = ft_strjoin(prefix, env->value);
	free(prefix);
	return (result);
}

/*
 * 转换：
 *
 * t_env:
 * key = "USER"
 * value = "chen"
 *
 * char **:
 * "USER=chen"
 *
 * value == NULL 的节点不会传给 execve。
 */
char	**env_to_array(t_env *env)
{
	char	**envp;
	int		i;

	envp = ft_calloc(count_exported_env(env) + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			envp[i] = env_node_to_string(env);
			if (!envp[i])
			{
				ft_free_arr(envp);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	return (envp);
}
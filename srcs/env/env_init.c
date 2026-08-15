/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:53:13 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 02:49:27 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_key_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

static t_env	*env_from_string(char *str)
{
	t_env	*node;
	char	*key;
	char	*value;
	int		len;

	len = env_key_len(str);
	key = ft_substr(str, 0, len);
	if (!key)
		return (NULL);
	value = NULL;
	if (str[len] == '=')
		value = ft_strdup(str + len + 1);
	if (str[len] == '=' && !value)
		return (free(key), NULL);
	node = env_new(key, value, 1);
	free(key);
	free(value);
	return (node);
}

t_env	*env_init(char **envp)
{
	t_env	*env;
	t_env	*node;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		node = env_from_string(envp[i]);
		if (!node)
			return (env_free(&env), NULL);
		env_add_back(&env, node);
		i++;
	}
	return (env);
}

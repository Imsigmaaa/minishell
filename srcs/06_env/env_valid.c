/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_valid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:56:39 by yushan            #+#    #+#             */
/*   Updated: 2026/06/14 23:57:13 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_is_alpha_uscore(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z') || c == '_');
}

static int	env_is_alnum_uscore(char c)
{
	return (env_is_alpha_uscore(c) || (c >= '0' && c <= '9'));
}

int	env_is_valid_key(char *key)
{
	int	i;

	if (!key || !key[0])
		return (0);
	if (!env_is_alpha_uscore(key[0]))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!env_is_alnum_uscore(key[i]))
			return (0);
		i++;
	}
	return (1);
}

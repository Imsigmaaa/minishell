/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:51:37 by yushan            #+#    #+#             */
/*   Updated: 2026/06/14 23:52:17 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_strlen(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

int	env_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (!s1 || !s2)
		return (s1 != s2);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*env_join_pair(char *key, char *value)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(env_strlen(key) + env_strlen(value) + 2);
	if (!str)
		return (NULL);
	i = 0;
	while (key[i])
	{
		str[i] = key[i];
		i++;
	}
	str[i++] = '=';
	j = 0;
	while (value && value[j])
		str[i++] = value[j++];
	str[i] = '\0';
	return (str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 23:57:31 by yushan            #+#    #+#             */
/*   Updated: 2026/06/14 23:58:09 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	env_putstr(char *s)
{
	while (s && *s)
	{
		write(1, s, 1);
		s++;
	}
}

void	env_print(t_env *env)
{
	while (env)
	{
		if (env->exported && env->value)
		{
			env_putstr(env->key);
			env_putstr("=");
			env_putstr(env->value);
			env_putstr("\n");
		}
		env = env->next;
	}
}

void	env_print_export(t_env *env)
{
	while (env)
	{
		if (env->exported)
		{
			env_putstr("declare -x ");
			env_putstr(env->key);
			if (env->value)
			{
				env_putstr("=\"");
				env_putstr(env->value);
				env_putstr("\"");
			}
			env_putstr("\n");
		}
		env = env->next;
	}
}

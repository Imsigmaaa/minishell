/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 14:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	equal_position(char *argument)
{
	int	i;

	i = 0;
	while (argument[i] && argument[i] != '=')
		i++;
	if (!argument[i])
		return (-1);
	return (i);
}

static int	export_identifier_error(char *argument)
{
	write(2, "minishell: export: `", 20);
	write(2, argument, ms_strlen(argument));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

static int	export_one(t_shell *shell, char *argument)
{
	char	*key;
	char	*value;
	int		equal;
	int		status;

	equal = equal_position(argument);
	if (equal >= 0)
		key = ms_substr(argument, 0, equal);
	else
		key = ms_strdup(argument);
	if (!key)
		return (perror("malloc"), 1);
	if (!env_is_valid_key(key))
		return (free(key), export_identifier_error(argument));
	value = NULL;
	if (equal >= 0)
		value = argument + equal + 1;
	status = !env_set(&shell->env, key, value, 1);
	free(key);
	if (status)
		perror("malloc");
	return (status);
}

int	builtin_export(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	status;

	if (!cmd->argv[1])
		return (print_export_sorted(shell->env), 0);
	i = 1;
	status = 0;
	while (cmd->argv[i])
	{
		if (export_one(shell, cmd->argv[i]))
			status = 1;
		i++;
	}
	return (status);
}

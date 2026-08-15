/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 14:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_shell *shell, t_cmd *cmd)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	name = cmd->argv[0];
	if (env_strcmp(name, "echo") == 0)
		return (builtin_echo(cmd));
	if (env_strcmp(name, "cd") == 0)
		return (builtin_cd(shell, cmd));
	if (env_strcmp(name, "pwd") == 0)
		return (builtin_pwd(cmd));
	if (env_strcmp(name, "export") == 0)
		return (builtin_export(shell, cmd));
	if (env_strcmp(name, "unset") == 0)
		return (builtin_unset(shell, cmd));
	if (env_strcmp(name, "env") == 0)
		return (builtin_env(shell, cmd));
	if (env_strcmp(name, "exit") == 0)
		return (builtin_exit(shell, cmd));
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/23 14:03:02 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_error(char *path)
{
	write(2, "minishell: cd: ", 15);
	perror(path);
	return (1);
}

static char	*cd_target(t_shell *shell, t_cmd *cmd)
{
	char	*target;

	if (cmd->argv[1])
		return (cmd->argv[1]);
	target = env_get(shell->env, "HOME");
	if (!target)
		write(2, "minishell: cd: HOME not set\n", 28);
	return (target);
}

static int	update_directories(t_shell *shell, char *old_pwd, char *new_pwd)
{
	if (old_pwd && !env_set(&shell->env, "OLDPWD", old_pwd, 1))
		return (0);
	if (new_pwd && !env_set(&shell->env, "PWD", new_pwd, 1))
		return (0);
	return (1);
}

int	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*target;
	char	*old_pwd;
	char	*new_pwd;
	int		status;

	if (cmd->argv[1] && cmd->argv[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 2);
	target = cd_target(shell, cmd);
	if (!target)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (chdir(target) < 0)
		return (free(old_pwd), cd_error(target));
	new_pwd = getcwd(NULL, 0);
	status = 0;
	if (!new_pwd)
		status = cd_error("getcwd");
	else if (!update_directories(shell, old_pwd, new_pwd))
		status = (perror("malloc"), 1);
	free(old_pwd);
	free(new_pwd);
	return (status);
}

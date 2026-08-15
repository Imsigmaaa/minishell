/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:15:51 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/16 03:34:19 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exec_error(t_cmd *cmd, int err_code)
{
	if (err_code == 1)
	{
		perror("malloc");
		return ;
	}
	if (err_code == 126)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else if (err_code == 127 && ft_strchr(cmd->argv[0], '/'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	else if (err_code == 127)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
}

void	exec_cmd(t_cmd *cmds, t_env *env)
{
	char	**envp;
	char	*path;
	int		err_code;

	path = get_exec_path(cmds->argv[0], env, &err_code);
	if (!path)
	{
		print_exec_error(cmds, err_code);
		exit(err_code);
	}
	envp = env_to_array(env);
	if (!envp)
	{
		free(path);
		perror("malloc");
		exit(1);
	}
	execve(path, cmds->argv, envp);
	perror("execve");
	env_free_array(envp);
	free(path);
	exit(126);
}

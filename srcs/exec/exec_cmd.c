/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:15:51 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/23 18:02:39 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_error(t_cmd *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(msg, 2);
}

void	print_exec_error(t_cmd *cmd, int err_code)
{
	struct stat	st;

	if (err_code == 1)
	{
		perror("malloc");
		return ;
	}
	if (err_code == 126)
	{
		if (stat(cmd->argv[0], &st) == 0 && S_ISDIR(st.st_mode))
			print_cmd_error(cmd, ": Is a directory\n");
		else
			print_cmd_error(cmd, ": Permission denied\n");
	}
	else if (err_code == 127 && ft_strchr(cmd->argv[0], '/'))
		print_cmd_error(cmd, ": No such file or directory\n");
	else if (err_code == 127)
		print_cmd_error(cmd, ": command not found\n");
}

int	exec_cmd(t_cmd *cmds, t_env *env)
{
	char	**envp;
	char	*path;
	int		err_code;

	path = get_exec_path(cmds->argv[0], env, &err_code);
	if (!path)
	{
		print_exec_error(cmds, err_code);
		return (err_code);
	}
	envp = env_to_array(env);
	if (!envp)
	{
		free(path);
		perror("malloc");
		return (1);
	}
	execve(path, cmds->argv, envp);
	perror("execve");
	env_free_array(envp);
	free(path);
	return (126);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:15:51 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:08:43 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if(s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i ++;
	}
	return (s1[i] - s2[i]);
}

int	is_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if(ft_strcmp(cmd->argv[0], "echo") == 0)
		return (1);
	if(ft_strcmp(cmd->argv[0], "cd") == 0)
		return (1);
	if(ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (1);
	if(ft_strcmp(cmd->argv[0], "export") == 0)
		return (1);
	if(ft_strcmp(cmd->argv[0], "unset") == 0)
		return (1);
	if(ft_strcmp(cmd->argv[0], "env") == 0)
		return (1);
	if(ft_strcmp(cmd->argv[0], "exit") == 0)
		return (1);
	return (0);
}

static	int	check_exec_error(char *path)
{
	struct stat st;
	// 先查文件存不存在
	if (access(path, F_OK) != 0)// F_OK 文件存不存在 file ok
		return (127);//文件不存在
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))// 查身份证，看是不是文件夹
		return (126);//找到了这个文件,但不能执行它，文件存在,但没有执行权限;或者是个目录
	if (access(path, X_OK) != 0)
		return (126);
	return (0);
}

static	char	*join_cmd_path(char *path, char *av)
{
	char *tmp;
	char *new_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)//失败返回NULL
		return (NULL);
	new_path = ft_strjoin(tmp, av);
	free(tmp);
	if (!new_path)
		return (NULL);
	return (new_path);
}

static	char	*find_valid_path(char *av, char **all_paths, int *err_code)
{
	int	i;
	int	status;
	int	no_permission;
	char *new_path;

	i = 0;
	no_permission = 0;
	while (all_paths[i])//遍历所有路径
	{
		new_path = join_cmd_path(all_paths[i], av);
		if(!new_path)
			return (*err_code = 1, NULL);
		status = check_exec_error(new_path);
		if (status == 0)//如果是可执行命令则返回路径
			return (new_path);
		if (status == 126)
			no_permission = 1;//找到文件但是没有权限或者是个文件夹
		free(new_path);//如果不可执行则free掉进行下一个循环寻找
		i ++;
	}
	*err_code = 127;
	if (no_permission == 1)
		*err_code = 126;
	return (NULL);//找不到返回NULL
}

static char	*get_path_from_env(char *av, t_env *env, int *err_code)
{
	t_env	*tmp;
	char	**all_paths;
	char	*path;
	
	tmp = env;
	while (tmp && ft_strcmp(tmp->key, "PATH") != 0)//找到key里是PATH的那个节点
		tmp = tmp->next;
	if (!tmp || !tmp->value)
	{
		*err_code = 127;
		return (NULL);
	}
	all_paths = ft_split(tmp->value, ':');//将节点value以冒号为标志拆分成路径组
	if (!all_paths)
	{
		*err_code = 1;
		return (NULL);
	}
	path = find_valid_path(av, all_paths, err_code);//找到正确的可执行路径
	ft_free_arr(all_paths);
	return (path);
}

static	char	*get_path(char *av, t_env *env, int *err_code)
{
	char	*path;
	
	*err_code = 0;
	path = NULL;
	if(ft_strchr(av, '/'))//如果执行命令里带有/(包括/ ./ ../)
	{
		*err_code = check_exec_error(av);
		if(*err_code == 0)
		{
			path = ft_strdup(av);
			if (!path)
				*err_code = 1;
		}
		return (path);// 如果err_code不是0直接返回NULL
	}
	path = get_path_from_env(av, env, err_code);
	return (path);
}

static	char	**env_to_array(t_env *env)
{
	t_env	*tmp;
	char	**envp;
	char	*tmp_envp;

	int	env_count = 0;
	tmp = env;
	if (!tmp)
		return (NULL);
	while (tmp)
	{
		env_count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (env_count + 1));
	if (!envp)
		return (NULL);
	tmp = env;
	int	i = 0;
	while (tmp)
	{
		tmp_envp = ft_strjoin(tmp->key, "=");
		if (!tmp_envp)
		{
			ft_free_arr(envp);
			return (NULL);
		}
		envp[i] = ft_strjoin(tmp_envp, tmp->value);
		if (!envp[i])
		{
			free(tmp_envp);
			ft_free_arr(envp);
			return (NULL);
		}
		free(tmp_envp);
		tmp = tmp->next;
		i ++;
	}
	envp[i] = NULL;
	return (envp);
	
}

void	exec_cmd(t_cmd *cmds, t_env *env)
{
	char	**envp;
	char	*path;
	int		err_code;

	path = get_path(cmds->argv[0], env, &err_code);
	if (!path)
	{
		if (err_code == 126)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmds->argv[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
		}
		else if (err_code == 127 && ft_strchr(cmds->argv[0], '/'))// 带 /,文件本身不存在
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmds->argv[0], 2);
			ft_putstr_fd(" No such file or directory\n", 2);
		}
		else if (err_code == 127)// 不带 /,PATH 里搜不到
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmds->argv[0], 2);
			ft_putstr_fd(" command not found\n", 2);
		}
		exit(err_code);//command no found
	}
	envp = env_to_array(env);
	execve(path, cmds->argv, envp);//如果execve执行成功下面的代码不会执行 反之
	perror("execve");
	ft_free_arr(envp);
	free(path);
	exit(126);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:15:51 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/08 23:39:44 by xingchen         ###   ########.fr       */
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
	//判断cmd是否存在 cmd->argv是否分配，cmd->argv[0]里有没有命令 ,
	//真正的shell没有命令的话直接回车不崩溃
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
//遍历所有路径找到可执行命令路径
char	*find_valid_path(char *av, char **all_paths)
{
	int	i;
	char *new_path;
	char *tmp;

	i = 0;
	//遍历所有路径
	while (all_paths[i])
	{
		//在路径后+ / + av即要执行的命令 比如/bin后面加/变成/bin/ 再+cat变成/bin/cat
		tmp = ft_strjoin(all_paths[i], "/");
		if (!tmp)//失败返回NULL
			return (NULL);
		new_path = ft_strjoin(tmp, av);
		free(tmp);
		if(!new_path)
			return (NULL);
		if (access(new_path, F_OK | X_OK) == 0)//如果是可执行命令则返回路径
			return (new_path);
		free(new_path);//如果不可执行则free掉进行下一个循环寻找
		i ++;
	}
	return (NULL);//找不到返回NULL
}
//得到可执行路径
char	*get_path(char *av, t_env *env)
{
	int		i;
	char	*path;
	char	**all_paths;
	t_env	*tmp;
	
	tmp = env;
	if(ft_strchr(av, '/'))//如果执行命令里带有/(包括/ ./ ../)
	{
		if(av && access(av, F_OK | X_OK) == 0)// 用户直接给了路径：./a.out 或 /bin/ls
			return(ft_strdup(av));
		return (NULL);// 有/但找不到 → 直接报错，不去PATH找
	}
	else
	{
		while (tmp && ft_strcmp(tmp->key, "PATH") != 0)//找到key里是PATH的那个节点
				tmp = tmp->next;
		if (!tmp)
			return (NULL);
		all_paths = ft_split(tmp->value, ':');//将节点value以冒号为标志拆分成路径组
		if (!all_paths)
			return (NULL);
		path = find_valid_path(av, all_paths);//找到正确的可执行路径
		ft_free_arr(all_paths);
		if(!path)
			return (NULL);
		return (path);
	}
}

void	exec_cmd(t_cmd *cmds, t_env *env)
{
	char	**envp;
	char	*path;

	path = get_path(cmds->argv[0], env);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", cmds->argv[0]);
		exit(127);//command no found
	}
	envp = env_to_array(env);
	execve(path, cmds->argv, envp);//如果execve执行成功下面的代码不会执行 反之
	perror("execve");
	ft_free_arr(envp);
	free(path);
}
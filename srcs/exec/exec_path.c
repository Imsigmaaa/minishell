/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 01:38:07 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/23 18:27:20 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static	int	check_exec_error(char *path)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
		return (127);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (126);
	if (access(path, X_OK) != 0)
		return (126);
	return (0);
}

static	char	*join_cmd_path(char *path, char *av)
{
	char	*tmp;
	char	*new_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	new_path = ft_strjoin(tmp, av);
	free(tmp);
	if (!new_path)
		return (NULL);
	return (new_path);
}

static	char	*find_valid_path(char *av, char **all_paths, int *err_code)
{
	int		i;
	int		status;
	int		no_permission;
	char	*new_path;

	i = 0;
	no_permission = 0;
	while (all_paths[i])
	{
		new_path = join_cmd_path(all_paths[i], av);
		if (!new_path)
			return (*err_code = 1, NULL);
		status = check_exec_error(new_path);
		if (status == 0)
			return (new_path);
		if (status == 126)
			no_permission = 1;
		free(new_path);
		i ++;
	}
	*err_code = 127;
	if (no_permission == 1)
		*err_code = 126;
	return (NULL);
}

static	char	*get_path_from_env(char *av, t_env *env, int *err_code)
{
	t_env	*tmp;
	char	**all_paths;
	char	*path;

	tmp = env;
	while (tmp && env_strcmp(tmp->key, "PATH") != 0)
		tmp = tmp->next;
	if (!tmp || !tmp->value)
	{
		*err_code = 127;
		return (NULL);
	}
	all_paths = split_exec_path(tmp->value);
	if (!all_paths)
	{
		*err_code = 1;
		return (NULL);
	}
	path = find_valid_path(av, all_paths, err_code);
	env_free_array(all_paths);
	return (path);
}

char	*get_exec_path(char *av, t_env *env, int *err_code)
{
	char	*path;

	*err_code = 0;
	if (!av || !av[0])
	{
		*err_code = 127;
		return (NULL);
	}
	path = NULL;
	if (ft_strchr(av, '/'))
	{
		*err_code = check_exec_error(av);
		if (*err_code == 0)
		{
			path = ft_strdup(av);
			if (!path)
				*err_code = 1;
		}
		return (path);
	}
	path = get_path_from_env(av, env, err_code);
	return (path);
}

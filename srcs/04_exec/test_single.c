/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 00:22:53 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/09 13:17:04 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!src && !dest && n != 0)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i ++;
	}
	return (dest);
}
char	*ft_strchr(const char *s, int c)
{
	char	*s1;
	size_t	i;

	s1 = (char *)s;
	i = 0;
	while (i <= ft_strlen(s1))
	{
		if ((s1[i]) == (char)c)
			return (s1 + i);
		i ++;
	}
	return (NULL);
}
char	*ft_strdup(const char *s)
{
	char	*dup;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(s);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i ++;
	}
	dup[i] = '\0';
	return (dup);
}

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i ++;
		if (s[i] && s[i] != c)
		{
			count ++;
			i ++;
			while (s[i] && s[i] != c)
				i ++;
		}
	}
	return (count);
}

static char	*cpy_words(char const *s, char c, size_t *dup_j_len)
{
	size_t	j;
	char	*temp;

	while (s[*dup_j_len] && s[*dup_j_len] != c)
		(*dup_j_len)++;
	temp = malloc(*dup_j_len + 1);
	if (!temp)
		return (NULL);
	j = 0;
	while (j < *dup_j_len)
	{
		temp[j] = s[j];
		j ++;
	}
	temp[j] = '\0';
	return (temp);
}

static	char	*fill_array(char **dup, char const *s, char c, size_t *index)
{
	size_t	dup_i_len;

	*dup = NULL;
	while (s[*index] && s[*index] == c)
		(*index)++;
	dup_i_len = 0;
	if (s[*index] && s[*index] != c)
	{
		(*dup) = cpy_words(s + *index, c, &dup_i_len);
		if (!(*dup))
			return (NULL);
	}
	*index = *index + dup_i_len;
	return (*dup);
}

char	**ft_split(char const *s, char c)
{
	char	**dup;
	size_t	dup_len;
	size_t	index;
	size_t	j;

	dup_len = count_words(s, c);
	dup = (char **)malloc(sizeof(char *) * (dup_len + 1));
	if (!dup)
		return (NULL);
	index = 0;
	j = 0;
	while (s[index] && j < dup_len)
	{
		fill_array(&dup[j], s, c, &index);
		if (!dup[j])
		{
			while (j > 0)
				free(dup [--j]);
			free(dup);
			return (NULL);
		}
		j ++;
	}
	dup[dup_len] = NULL;
	return (dup);
}
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s2_len;
	size_t	s1_len;
	char	*dup;

	if (!s1)
		s1_len = 0;
	else
		s1_len = ft_strlen(s1);
	if (!s2)
		s2_len = 0;
	else
		s2_len = ft_strlen(s2);
	dup = malloc(s1_len + s2_len + 1);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s1, s1_len);
	ft_memcpy(dup + s1_len, s2, s2_len);
	dup[s1_len + s2_len] = '\0';
	return (dup);
}

void	ft_free_arr(char **arr)
{
	int	i;

	i = 0;
	if(!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static	int	count_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		count;

	tmp = cmds;
	count = 0;
	while (tmp)
	{
		tmp = tmp->next;
		count ++;
	}
	return (count);
}
static	int	ft_strcmp(const char *s1, const char *s2)
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

static	int	is_builtin(t_cmd *cmd)
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
char	*find_valid_path(char *av, char **all_paths)
{
	int	i;
	char *new_path;
	char *tmp;

	i = 0;
	while (all_paths[i])
	{
		tmp = ft_strjoin(all_paths[i], "/");
		if (!tmp)
			return (NULL);
		new_path = ft_strjoin(tmp, av);
		free(tmp);
		if(!new_path)
			return (NULL);
		if (access(new_path, F_OK | X_OK) == 0)
			return (new_path);
		free(new_path);
		i ++;
	}
	return (NULL);
}
char	*get_path(char *av, t_env *env)
{
	int		i;
	char	*path;
	char	**all_paths;
	t_env	*tmp;
	
	tmp = env;
	if(ft_strchr(av, '/'))
	{
		if(av && access(av, F_OK | X_OK) == 0)
			return(ft_strdup(av));
		return (NULL);
	}
	else
	{
		while (tmp && strcmp(tmp->key, "PATH") != 0)
				tmp = tmp->next;
		if (!tmp)
			return (NULL);
		all_paths = ft_split(tmp->value, ':');
		if (!all_paths)
			return (NULL);
		path = find_valid_path(av, all_paths);
		ft_free_arr(all_paths);
		if(!path)
			return (NULL);
		return (path);
	}
}
void	exec_single(t_shell *shell,t_cmd *cmds, t_env *env)
{
	char	*path;
	char	**envp;
	pid_t	pid;
	int		status;
	if(is_builtin(cmds))
	{
		shell->exit_status = exec_builtin(cmds, env);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		return ;
	}	
	if (pid == 0)
	{
		if (cmds->redirs && exec_redir(cmds) == -1)
			exit(1);
		exec_cmd(cmds, env);
	}
	waitpid(pid, &status, 0);
	if(WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return ;

}
int	executor(t_shell *shell,t_cmd *cmds, t_env *env)
{
	if (!cmds)
		return (1);
	if (count_cmds(cmds) == 1)
		exec_single(shell,cmds, env);
	return 0;
}

int main(void)
{
	t_cmd cmd;

	cmd.argv = (char *[]) {"unset", NULL};
	cmd.redirs = NULL;
	cmd.next = NULL;

	executor(&cmd, NULL);
}/*

echo hello | grep hello > out.txt

cmd1 = echo hello
cmd2 = grep hello > out.txt

exec_pipe()
      │
	fork()
   ┌──┴──┐
Child1  Child2
   │        │
builtin   dup2()
           │
        exec_redir()
          │
         exec_cmd()






Child 1                    Child 2
┌──────────┐      pipe      ┌────────────────────────┐
│ echo     │ ─────────────▶ │ grep hello            │
│ builtin  │                │ exec_redir()          │
└──────────┘                │ exec_cmd()            │
                            └────────────────────────┘
                                     │
                                     ▼
                                  out.txt




								  */
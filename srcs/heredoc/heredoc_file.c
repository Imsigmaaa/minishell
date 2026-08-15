/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 00:08:47 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/16 00:35:45 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_heredoc_path(int number)
{
	char	*num;
	char	*path;

	num = ft_itoa(number);
	if (!num)
		return (NULL);
	path = ft_strjoin("/tmp/heredoc_tmp_", num);
	free(num);
	return (path);
}

int	open_heredoc_file(char **path, int number)
{
	int	fd;
	int	saved_errno;

	while (1)
	{
		*path = get_heredoc_path(number);
		if (!*path)
			return (errno = ENOMEM, -1);
		fd = open(*path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
			return (fd);
		saved_errno = errno;
		free(*path);
		*path = NULL;
		if (saved_errno != EEXIST)
			return (errno = saved_errno, -1);
		number ++;
	}
}

void	print_heredoc_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

void	remove_heredoc_file(char *path)
{
	if (!path)
		return ;
	unlink(path);
	free(path);
}

int	open_heredoc_read(t_shell *shell, t_redir *redir, char *path)
{
	redir->heredoc_fd = open(path, O_RDONLY);
	remove_heredoc_file(path);
	if (redir->heredoc_fd == -1)
	{
		perror("heredoc");
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}

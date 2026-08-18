/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/18 04:43:40 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_cmd *cmd)
{
	char	*directory;

	(void)cmd;
	directory = getcwd(NULL, 0);
	if (!directory)
		return (perror("pwd"), 1);
	write(1, directory, ft_strlen(directory));
	write(1, "\n", 1);
	free(directory);
	return (0);
}

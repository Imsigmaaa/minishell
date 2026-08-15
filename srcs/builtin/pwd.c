/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 14:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_cmd *cmd)
{
	char	*directory;

	if (cmd->argv[1])
		return (write(2, "minishell: pwd: too many arguments\n", 35), 1);
	directory = getcwd(NULL, 0);
	if (!directory)
		return (perror("pwd"), 1);
	write(1, directory, ms_strlen(directory));
	write(1, "\n", 1);
	free(directory);
	return (0);
}

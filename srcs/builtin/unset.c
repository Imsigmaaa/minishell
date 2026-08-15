/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 02:48:57 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_identifier_error(char *argument)
{
	write(2, "minishell: unset: `", 19);
	write(2, argument, ft_strlen(argument));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

int	builtin_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (cmd->argv[i])
	{
		if (!env_is_valid_key(cmd->argv[i]))
			status = unset_identifier_error(cmd->argv[i]);
		else if (!env_unset(&shell->env, cmd->argv[i]))
			status = (perror("malloc"), 1);
		i++;
	}
	return (status);
}

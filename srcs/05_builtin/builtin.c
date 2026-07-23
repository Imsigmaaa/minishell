/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 19:21:05 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/22 15:12:46 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(t_cmd *cmd)
{
	int	i;

	i  = 1;
	while (cmd->argv[i])
	{
		ft_printf("%s",cmd->argv[i]);
		i ++;
		if (cmd->argv[i])
			ft_printf(" ");
	}
	ft_printf("\n");
	return ;
}
int	exec_builtin(t_shell *shell, t_cmd *cmd)
{
	int	status;

	if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
	if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
		if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
		if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
		if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
		if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
		if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
		if (cmd->argv[0] == 'echo')
		builtin_echo(cmd->argv);
		
}
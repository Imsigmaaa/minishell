/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 03:25:57 by xingchen          #+#    #+#             */
/*   Updated: 2026/08/24 19:11:59 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	print_child_signal(int status)
{
	int	signal_number;

	if (!WIFSIGNALED(status))
		return ;
	signal_number = WTERMSIG(status);
	if (signal_number == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (signal_number == SIGQUIT)
		write(STDERR_FILENO, "Quit: 3\n", 8);
}

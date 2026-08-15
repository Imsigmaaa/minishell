/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 13:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/15 13:00:00 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

int	expansion(t_shell *shell)
{
	int	result;

	if (!expand_all_argv(shell))
		return (perror("malloc"), shell->exit_status = 1, 0);
	result = expand_all_redirs(shell);
	if (result == 0)
		return (perror("malloc"), shell->exit_status = 1, 0);
	if (result < 0)
		return (0);
	return (1);
}

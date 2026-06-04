/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:28:17 by yushan            #+#    #+#             */
/*   Updated: 2026/06/04 14:28:36 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

void    init_shell(t_shell *shell, char **envp)
{
    shell->env = copy_environment(envp);
    shell->tokens = NULL;
    shell->commands = NULL;
    shell->exit_status = 0;
    shell->should_exit = 0;
}

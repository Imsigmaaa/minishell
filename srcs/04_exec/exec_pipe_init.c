/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 23:25:23 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/22 14:03:08 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_exec(t_exec *exec)
{
	exec->cmd_count = 0;
	exec->pipe_fd = NULL;
	exec->pids = NULL;
	exec->saved_stdin = - 1;
	exec->saved_stdout = -1;
	exec->status = 0;
}

// Contient toutes les informations pour l'exécution.
// 保存执行需要的所有信息。
int	init_exec_data(t_shell *shell, t_exec *exec)
{
	// Initialise la structure.
	// 初始化结构体。
	init_exec(exec);
	// Compte le nombre de commandes.
	// 统计命令数量。
	exec->cmd_count = count_cmds(shell->cmds);
	// Alloue la mémoire pour les pid.
	// 给所有子进程 pid 分配空间。
	exec->pids = malloc(sizeof(pid_t) * exec->cmd_count);
	if (!exec->pids)
	{
		perror("malloc");
		shell->exit_status = 1;
		return (0);
	}
	// Alloue la mémoire pour les pipes.
	// 给所有 pipe 分配空间。
	if (exec->cmd_count > 1)
	{
		exec->pipe_fd = malloc(sizeof(int [2]) * (exec->cmd_count - 1));
		if (!exec->pipe_fd)
		{
			perror("malloc");
			free(exec->pids);//释放
			exec->pids = NULL;
			shell->exit_status = 1;
			return (0);
		}
	}
	return (1);
}

int	create_pipes(t_shell *shell, t_exec *exec)
{
	int		i;

	i = 0;
	// Crée tous les pipes.
	//循环创建所有管道cmd_count - 1根
	while (i < exec->cmd_count - 1)
	{
		// Crée un pipe.
		// 创建一根管道。
		if (pipe(exec->pipe_fd[i]) == -1)
		{
			close_created_fd(exec, i);//释放所有已创建pipe
			perror("pipe");
			//凡是父进程提前 return 的错误路径，
			//都应该考虑设置 shell->exit_status = 1
			shell->exit_status = 1;
			return (0);
		}
		i ++;
	}
	return (1);
}
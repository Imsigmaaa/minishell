/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:17:37 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/06 00:57:39 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
/bin/echo hello | /bin/cat | /usr/bin/wc -c
echo -- écrit"hello\n"-->fd[0][1](écriture)-->pipe(buffer : "hello\n")-->fd[0][0](lecture)--lit"hello\n"-->cat
（echo 通过 fd[0][1] 将结果写入管道，数据存放在管道缓冲区中，然后 cat 通过 fd[0][0] 读取这些数据。）
Le premier processus écrit dans le pipe, le deuxième lit depuis le pipe.
cat读完数据通过fd[1][1]将结果写入第二个管道，数据存放在第二个管道缓冲区,然后wc通过fd[1][0]读取这些数据
echo écrit son résultat dans le pipe via fd[0][1]. Les données sont stockées dans le buffer du pipe. 
Ensuite, cat lit ces données via fd[0][0].
*/
void close_created_fd(t_exec *exec, int created_pipes)
{
	int	j;
	j = 0;
	while (j < created_pipes)
	{
		close(exec->pipe_fd[j][0]);
		close(exec->pipe_fd[j][1]);
		j ++;
	}
}

static void	exec_child(t_cmd *cmd, t_env *env, int i, t_exec *exec)
{
	//如果当前是第一个命令
	// Si c'est la première commande (ex : echo)
	if (!cmd->redirs)
	{
	if(i == 0)
	{	
		// La commande écrit dans le premier pipe.
		// 第一条命令把结果写到第一根管道。
		if(dup2(exec->pipe_fd[0][1], STDOUT_FILENO) ==-1)
		{// Si dup2 échoue, on ferme tout et on quitte.
			// 如果 dup2 失败，关闭资源并退出子进程。
			close_created_fd(exec, exec->cmd_count - 1);
			exit(1);
		}
	}
	// Si c'est la dernière commande (ex : wc).
	// 如果这是最后一条命令（例如：wc）。
	else if(i == exec->cmd_count - 1)
	{
		// La commande lit les données depuis le pipe précédent.
		// 最后一条命令从上一根管道读取数据。
		if(dup2(exec->pipe_fd[i- 1][0], STDIN_FILENO)== -1)
		{
			close_created_fd(exec, exec->cmd_count - 1);
			exit(1);
		}
	}
	// Sinon, c'est une commande au milieu (ex : cat).
	// 否则就是中间命令（例如：cat）。
	else
	{
		// La commande lit les données depuis le pipe précédent.
		// 中间命令先从上一根管道读取数据。

		// Puis elle écrit son résultat dans le pipe suivant.
		// 然后把自己的结果写到下一根管道。。
		if(dup2(exec->pipe_fd[i - 1][0], STDIN_FILENO) == -1
		|| dup2(exec->pipe_fd[i][1], STDOUT_FILENO) == -1 )
		{
			close_created_fd(exec, exec->cmd_count - 1);
			exit(1);
		}
	}
}
else
	exec_redir(cmd);
	// Les redirections sont terminées.
	// 重定向已经完成。

	// Les pipes ne servent plus.
	// 原来的 pipe 文件描述符已经没有用了。

	// On les ferme pour éviter les leaks.
	// 关闭它们，避免文件描述符泄漏。
	/*
	我们复制cmd_count个进程，每个进程都是从父进程复制而来，
	在父进程里我们创建了cmds_count-1管道，
	由于 fork 会复制父进程打开的文件描述符，
	所以每个子进程里都有cmds_count-1个管道，
	所以每次dup2要关闭所有管道（不管用到没有）
	*/
	close_created_fd(exec, exec->cmd_count - 1);
	// Exécute réellement la commande.
	// 真正执行命令。

	// Soit un builtin, soit execve pour une commande externe.
	// 如果是 builtin 就执行 builtin，否则调用 execve 执行外部命令。
	exec_cmd(cmd, env);
}

void	init_exec(t_exec *exec)
{
	exec->cmd_count = 0;
	exec->pipe_fd = NULL;
	exec->pids = NULL;
	exec->saved_stdin = - 1;
	exec->saved_stdout = - 1;
	exec->status = 0;
}
// Exécute plusieurs commandes avec des pipes.
// 执行多个带管道的命令。
void	exec_pipe(t_shell *shell, t_cmd *cmds, t_env *env)
{
	int		i;
	t_cmd	*cur;

	// Contient toutes les informations pour l'exécution.
	// 保存执行需要的所有信息。
	t_exec	exec;
	// Initialise la structure.
	// 初始化结构体。
	init_exec(&exec);
	// Compte le nombre de commandes.
	// 统计命令数量。
	exec.cmd_count = count_cmds(cmds);
	// Alloue la mémoire pour les pid.
	// 给所有子进程 pid 分配空间。
	exec.pids = malloc(sizeof(pid_t) * exec.cmd_count);
	if (!exec.pids)
	{
		// Si malloc échoue.
		// malloc 失败。
		perror("malloc");
		return ;
	}
	// Alloue la mémoire pour les pipes.
	// 给所有 pipe 分配空间。
	exec.pipe_fd = malloc(sizeof(int [2]) * (exec.cmd_count - 1));
	if (!exec.pipe_fd)
	{
		free(exec.pids);
		perror("malloc");
		return ;
	}

	// Commence par la première commande.
	// 从第一条命令开始。
	cur = cmds;
	i = 0;
	// Crée tous les pipes.
	//循环创建所有管道
	while (i < exec.cmd_count - 1)
	{
		// Crée un pipe.
		// 创建一根管道。
		if (pipe(exec.pipe_fd[i]) == -1)
		{
			// Libère la mémoire si pipe échoue.
			// pipe 创建失败，释放资源。
			close_created_fd(&exec, i);
			free(exec.pids);
			free(exec.pipe_fd);
			perror("pipe");
			return ;
		}
		i ++;
	}
	i = 0;
	// Crée un processus pour chaque commande.
	// 每条命令创建一个子进程。
	while (i < exec.cmd_count)
	{
		// Crée un processus enfant.
		// 创建子进程。
		exec.pids[i] = fork();
		// Si fork échoue.
		// fork 失败。
		if (exec.pids[i] == -1)
		{
			close_created_fd(&exec, exec.cmd_count - 1);
			free(exec.pids);
			free(exec.pipe_fd);
			perror("fork");
			return ;
		}
		// Si on est dans le processus enfant.
		// 如果现在是子进程。
		if (exec.pids[i] == 0)
			// Configure les pipes puis exécute la commande.
			// 设置 pipe，然后执行命令。
			exec_child(cur, env, i, &exec);
		// Passe à la commande suivante.
		// 移动到下一条命令。
		cur = cur->next;
		i ++;
	}
	// Le parent n'utilise plus les pipes.
	// 父进程已经不用 pipe 了。
	close_created_fd(&exec, exec.cmd_count - 1);
	i = 0;
	// Attend tous les processus.
	// 等待所有子进程结束。
	while (i < exec.cmd_count)
	{
		// Attend le processus i.
		// 等待第 i 个子进程。
		if(waitpid(exec.pids[i], &exec.status, 0) == -1)
			perror("waitpid");
		// Garde seulement le code de retour de la dernière commande.
		// 只保存最后一个命令的退出码。
		if(i == exec.cmd_count - 1 && WIFEXITED(exec.status))
			shell->exit_status = WEXITSTATUS(exec.status);
		i ++;
	}
	// Libère les pid.
	// 释放 pid 数组
	free(exec.pids);
	// Libère les pipes.
	// 释放 pipe 数组。
	free(exec.pipe_fd);
	return ;
}	


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:17:37 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/15 16:11:26 by xingchen         ###   ########.fr       */
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

static int	dup_pipe_fd(int i, t_exec *exec)
{
	//如果当前是第一个命令
	// Si c'est la première commande (ex : echo)
	if(i == 0)
	{	
		// La commande écrit dans le premier pipe.
		// 第一条命令把结果写到第一根管道。
		if(dup2(exec->pipe_fd[0][1], STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	// Si c'est la dernière commande (ex : wc).
	// 如果这是最后一条命令（例如：wc）。
	else if(i == exec->cmd_count - 1)
	{
		// La commande lit les données depuis le pipe précédent.
		// 最后一条命令从上一根管道读取数据。
		if(dup2(exec->pipe_fd[i- 1][0], STDIN_FILENO)== -1)
			return (perror("dup2"), 0);
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
			return (perror("dup2"), 0);
	}
	return (1);
}
static void	exec_child(t_cmd *cmd, t_env *env, int i, t_exec *exec)
{
	int	status;
	// Si dup2 échoue, on ferme tout ,free tout et on quitte.
	// 如果 dup2 失败，关闭资源并释放退出子进程
	if (!dup_pipe_fd(i, exec))
	{
		close_created_fd(exec, exec->cmd_count - 1);
		free_exec(exec);
		exit(1);
	}
	// Si c'est une redirection et il échoue, on ferme tout ,free tout et on quitte.
	// 如果 是重定向且exec_redir 失败，关闭资源并释放退出子进程
	if (cmd->redirs && exec_redir(cmd) == -1)
	{
    	close_created_fd(exec, exec->cmd_count - 1);
		free_exec(exec);
    	exit(1);
	}
	// Les redirections sont terminées.
	// 重定向已经完成。
	// Les pipes ne servent plus.On les ferme pour éviter les leaks.
	// 原来的 pipe 文件描述符已经没有用了。关闭它们，避免文件描述符泄漏。
	/*
	我们复制cmd_count个进程，每个进程都是从父进程复制而来，
	在父进程里我们创建了cmds_count-1管道，
	由于 fork 会复制父进程打开的文件描述符，
	所以每个子进程里都有cmds_count-1个管道，
	所以每次dup2要关闭所有管道（不管用到没有）
	*/
	close_created_fd(exec, exec->cmd_count - 1);
	if (is_builtin(cmd))
    {	status = exec_builtin(cmd, env);
		free_exec(exec);
		exit(status);
	}
	// Exécute réellement la commande.
	// 真正执行命令。

	// Soit un builtin, soit execve pour une commande externe.
	// 如果是 builtin 就执行 builtin，否则调用 execve 执行外部命令。
	exec_cmd(cmd, env);
	exit(126);
}


// Attend tous les processus.
// 等待所有子进程结束。
static	void	wait_all_childs(t_shell *shell, t_exec *exec, int count)
{
	int	wait_error;
	int	i;

	wait_error = 0;//初始化等待的子进程出现错误=0
	i = 0;
	// Attend le processus i.
	// 等待第 i 个子进程。

	// Garde seulement le code de retour de la dernière commande.
	// 只保存最后一个命令的退出码。
	while (i < count)
	{
		if(waitpid(exec->pids[i], &exec->status, 0) == -1)
		{
			perror("waitpid");
			wait_error = 1;
		}
		else if(i == count - 1 )
			update_exit_status(shell, exec->status);
		i ++;
	}
	if (wait_error)
		shell->exit_status = 1;
	return ;
}
static	int	fork_children(t_shell *shell, t_exec *exec)
{
	int		i;
	t_cmd	*cur;
	
	i = 0;
	cur = shell->cmds;
	// Crée un processus pour chaque commande.
	// 每条命令创建一个子进程。
	while (i < exec->cmd_count)
	{
		// Crée un processus enfant.
		// 创建子进程。
		exec->pids[i] = fork();
		if (exec->pids[i] == -1)
		{
			perror("fork");
			close_created_fd(exec, exec->cmd_count - 1);
			wait_all_childs(shell, exec, i);
			shell->exit_status = 1;
			return (0);
		}
		// Si on est dans le processus enfant.
		// 如果现在是子进程。
		if (exec->pids[i] == 0)
			// Configure les pipes puis exécute la commande.
			// 设置 pipe，然后执行命令。
			exec_child(cur, shell->env, i, exec);
		// Passe à la commande suivante.
		// 移动到下一条命令。
		cur = cur->next;
		i ++;
	}
	return (1);
}

// Exécute plusieurs commandes avec des pipes.
// 执行多个带管道的命令。
void	exec_pipe(t_shell *shell)
{
	t_exec	exec;
	//初始化exec数据，创建所有pipes
	if (!init_exec_data(shell, &exec) || !create_pipes(shell, &exec))
	{
		//如果出错释放exec
		free_exec(&exec);
		return ;//父进程直接返回
	}
	//fork子进程
	if (!fork_children(shell, &exec))
	{
		//如果出错释放exec
		free_exec(&exec);
		return ;//父进程直接返回
	}
	// Le parent n'utilise plus les pipes.
	// 父进程已经不用 pipe 了。
	close_created_fd(&exec, exec.cmd_count - 1);
	wait_all_childs(shell, &exec, exec.cmd_count);
	
	// Libère la liste exec
	// 释放 exec列表
	free_exec(&exec);
}	


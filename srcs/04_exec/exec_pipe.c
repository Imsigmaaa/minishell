/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:17:37 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:20:08 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup_pipe_fd(int i, t_exec *exec)
{
	if (i == 0)
	{	
		if (dup2(exec->pipe_fd[0][1], STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	else if (i == exec->cmd_count - 1)
	{
		if(dup2(exec->pipe_fd[i- 1][0], STDIN_FILENO)== -1)
			return (perror("dup2"), 0);
	}
	else
	{
		if (dup2(exec->pipe_fd[i - 1][0], STDIN_FILENO) == -1
		|| dup2(exec->pipe_fd[i][1], STDOUT_FILENO) == -1 )
			return (perror("dup2"), 0);
	}
	return (1);
}

void	close_free_and_exit_child(t_exec *exec)
{
	close_created_fd(exec, exec->cmd_count - 1);
	free_exec(exec);
	exit(1);
}

static void	exec_child(t_shell *shell,t_cmd *cmd,  int i, t_exec *exec)
{
	int	status;
	if (!dup_pipe_fd(i, exec))
		close_free_and_exit_child(exec);
	if (cmd->redirs && exec_redir(cmd) == -1)
		close_free_and_exit_child(exec);
	close_created_fd(exec, exec->cmd_count - 1);
	if (!cmd->argv || !cmd->argv[0])//Pipeline 中只有重定向、没有命令时直接释放退出
	{
		free_exec(exec);
    	exit(0);//如果只有重定向，并且前面的 exec_redir() 已经成功，应该退出 0，不是 1
	}
	if (is_builtin(cmd))
    {	
		status = exec_builtin(shell, cmd);
		free_exec(exec);
		exit(status);
	}
	free_exec(exec);
	exec_cmd(cmd, shell->env);
	exit(126);
}

static	void	wait_all_childs(t_shell *shell, t_exec *exec, int count)
{
	int	wait_error;
	int	i;

	wait_error = 0;//初始化等待的子进程出现错误=0
	i = 0;
	while (i < count)
	{
		if (waitpid(exec->pids[i], &exec->status, 0) == -1)
		{
			perror("waitpid");
			wait_error = 1;
		}
		else if (i == count - 1 )
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
	while (i < exec->cmd_count)
	{
		exec->pids[i] = fork();
		if (exec->pids[i] == -1)
		{
			perror("fork");
			close_created_fd(exec, exec->cmd_count - 1);
			wait_all_childs(shell, exec, i);
			shell->exit_status = 1;
			return (0);
		}
		if (exec->pids[i] == 0)
			exec_child(shell,cur, i, exec);
		cur = cur->next;
		i ++;
	}
	return (1);
}

void	exec_pipe(t_shell *shell)
{
	t_exec	exec;

	if (!init_exec_data(shell, &exec) || !create_pipes(shell, &exec))
	{
		free_exec(&exec);
		return ;//父进程直接返回
	}
	if (!fork_children(shell, &exec))
	{
		free_exec(&exec);
		return ;//父进程直接返回
	}
	close_created_fd(&exec, exec.cmd_count - 1);
	wait_all_childs(shell, &exec, exec.cmd_count);
	free_exec(&exec);
}	

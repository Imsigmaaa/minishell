/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 23:26:44 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/22 16:50:39 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	free_exec(t_exec *exec)
{
	if (exec->pids)
		free(exec->pids);
	if (exec->pipe_fd)
		free(exec->pipe_fd);
	exec->pids = NULL;
	exec->pipe_fd = NULL;
	return ;
}
void	update_exit_status(t_shell *shell, int status)
{
	//如果子进程是正常结束的(只要子进程是通过 exit()
	//或者 return 从 main 返回，本质上也会调用 exit结束的，就属于正常结束（WIFEXITED）)
	if (WIFEXITED(status))
		//取出真正的退出码
		shell->exit_status = WEXITSTATUS(status);
	//子进程是不是被信号杀死了？(Ctrl+C（SIGINT）||kill -9||段错误（SIGSEGV）)
	else if(WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);//WTERMSIG(status) —— 是取具体信号编号(问的是"被哪个信号杀死的?",比如 SIGINT=2, SIGSEGV=11)
		/*WIFSTOPPED(status) 进程没有死，只是暂停了。
		WIFCONTINUED(status) 暂停后继续
		属于Job Control（作业控制）
		
		
		例如 subject 要求：

Ctrl+C
Ctrl+\
Ctrl+D
pipes
redirections
heredoc
builtins
$?

但是没有要求实现：

jobs
fg
bg
&（后台运行）
Ctrl+Z 的作业控制

也没有提到需要处理进程暂停或继续*/
}

int	count_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		count;

	tmp = cmds;
	count = 0;
	while (tmp)
	{
		tmp = tmp->next;
		count ++;
	}
	return (count);
}
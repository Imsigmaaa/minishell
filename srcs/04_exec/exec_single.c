/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:12:35 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/08 20:55:49 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

//解决单命令问题
void	exec_single(t_shell *shell, t_cmd *cmds, t_env *env)
{
	
	pid_t	pid;
	int		status;
	//如果av[0]是builtin则执行builtin函数 不是则执行常规函数
	if(is_builtin(cmds))
	{
		shell->exit_status = exec_builtin(cmds, env);
		return ;
	}
	/*fork()复制一个子进程成功的话返回0 
	失败返回-1，
	但是父进程成功的话返回的是子进程的PID比如4242
	*/
	//创建一个子进程来执行命令这样不会替换掉minishell
	/*我对shell的理解：
	shell相当于一个工具（命令解释器）（bash zsh minishell都算shell）
	为了执行外部命令shell会fork一个子进程，执行完子进程退出，父进程仍然是shell等待执行下一条命令
	如果不复制子进程直接在父进程里执行，那么shell会被替换成该命令，命令结束后也就消失了
	*/
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		/*exit_status 保存的是上一条命令的退出码（也就是 echo $? 显示的值）。
		常见约定是：
		0：成功
		1：一般错误（general error）
		126：找到文件，但不能执行
		127：命令不存在
		128 + signal：被信号终止（例如 Ctrl+C）
		所以：
		fork()如果失败了，说明这条命令没有成功执行。
	最常见的做法就是：
	shell->exit_status = 1;表示"发生了一个普通错误"。*/
		return ;
	}	
	if (pid == 0)
	{
		if (cmds->redirs && exec_redir(cmds) == -1)
			exit(1);
		exec_cmd(cmds, env);
	}
	waitpid(pid, &status, 0);
	/*status：相当于生成一个条形码（一堆数字） 
	用WIFEXITED(status)相当于一个扫码枪来验证status，
	如果是true（比如快递正常送达是true，false是快递异常）*/
	if(WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	return ;
	/*execve 就三件事：
去 /bin/ls 把这个程序加载进内存
把 argv 整个数组传给它（程序里的 main(int ac, char **av) 收到的就是这个）
把 envp 传给它作为环境变量
然后当前进程就变成这个程序了，原来的代码消失。
你可以把它理解成：用另一个程序替换掉自己。*/
}

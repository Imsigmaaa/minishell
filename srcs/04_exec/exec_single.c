/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:12:35 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/22 17:16:08 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//处理 builtin 命令 + 可能存在的重定向
//因为 builtin 是在 shell 自己的进程里执行的（没有 fork）
//如果不手动保存/恢复 fd，重定向一旦发生就会永久性地
//把 shell 自身的 stdout/stdin 改掉，导致之后所有输出都跑偏
static	int	saved_stdio(int	*saved_stdin, int *saved_stdout)
{
	//在动 fd 之前，先把原始的 stdout / stdin 复制一份备份起来
	//相当于"多接一根备用水管"，等下要用它把水管接回终端
	*saved_stdin = dup(STDIN_FILENO);
	//dup(oldfd) —— 自动找一个新的空闲 fd 来复制;意思是:"复制一份 fd 1(stdout)的内容,系统随便挑一个空闲的号,比如挑中了 fd 3,那么 saved_stdout 里存的值就是 3"。
	*saved_stdout = dup(STDOUT_FILENO);
	if(*saved_stdout == -1 || *saved_stdin == -1)
	{
		perror("dup");
		if (*saved_stdout >= 0)
			close(*saved_stdout);
		if (*saved_stdin >= 0)
			close(*saved_stdin);
		return (0);
	}
	return (1);
}

static	void	restore_stdio(int	saved_stdin, int saved_stdout)
{
	//不管上面走了哪条路径，执行完都要把 fd 改回来
	//用备份的 fd 覆盖回 STDOUT_FILENO / STDIN_FILENO
	//相当于把水管重新接回终端
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	//备份用的 fd 已经用完了，及时关闭，避免 fd 泄漏
	close(saved_stdout);
	close(saved_stdin);
}

static	void	exec_parent_action(t_shell *shell)
{
	int	redir_status;
	
	redir_status = 0;//初始化为0
	if (shell->cmds->redirs)//如果有重定向得到重定向状态status
		redir_status = exec_redir(shell->cmds);
	if (redir_status == -1)
    {
		shell->exit_status = 1;//检查重定向是否失败，失败就停止
		return ;
	}
	//检查有没有命令
	if (!shell->cmds->argv || !shell->cmds->argv[0])
	{
		shell->exit_status = 0;//重定向成功，但没有命令，所以状态为 0
		return ;//返回
	}
	//如果前面没有重定向或者重定向成功且有命令 则执行builtin
	shell->exit_status = exec_builtin(shell, shell->cmds);
}

static	void	exec_in_parent(t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!saved_stdio(&saved_stdin, &saved_stdout))
	{	
		shell->exit_status = 1;
		return ;
	}
	exec_parent_action(shell);
	restore_stdio(saved_stdin, saved_stdout);
}

//解决单命令问题
void	exec_single(t_shell *shell)
{
	
	pid_t	pid;
	int		status;
	//没有命令但可能只有重定向或者是builtin
	if (!shell->cmds->argv || !shell->cmds->argv[0] || is_builtin(shell->cmds))
	{
		exec_in_parent(shell);
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
		if (shell->cmds->redirs && exec_redir(shell->cmds) == -1)
			exit(1);
		exec_cmd(shell->cmds, shell->env);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		shell->exit_status = 1;
		return ;
	}
	/*status：相当于生成一个条形码（一堆数字） 
	用WIFEXITED(status)相当于一个扫码枪来验证status，
	如果是true（比如快递正常送达是true，false是快递异常）*/
	update_exit_status(shell, status);
}
/*execve 就三件事：
去 /bin/ls 把这个程序加载进内存
把 argv 整个数组传给它（程序里的 main(int ac, char **av) 收到的就是这个）
把 envp 传给它作为环境变量
然后当前进程就变成这个程序了，原来的代码消失。
你可以把它理解成：用另一个程序替换掉自己。*/
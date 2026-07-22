/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:12:35 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/22 12:45:41 by xingchen         ###   ########.fr       */
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
//处理 builtin 命令 + 可能存在的重定向
//因为 builtin 是在 shell 自己的进程里执行的（没有 fork）
//如果不手动保存/恢复 fd，重定向一旦发生就会永久性地
//把 shell 自身的 stdout/stdin 改掉，导致之后所有输出都跑偏
static	void	exec_builtin_with_redir(t_shell *shell)
{
	int	saved_stdout;
	int	saved_stdin;

	//在动 fd 之前，先把原始的 stdout / stdin 复制一份备份起来
	//相当于"多接一根备用水管"，等下要用它把水管接回终端
	saved_stdout = dup(STDOUT_FILENO);
	//dup(oldfd) —— 自动找一个新的空闲 fd 来复制;意思是:"复制一份 fd 1(stdout)的内容,系统随便挑一个空闲的号,比如挑中了 fd 3,那么 saved_stdout 里存的值就是 3"。
	saved_stdin = dup(STDIN_FILENO);
	//如果这条命令带重定向，就执行重定向（会把 fd 改接到文件上）
	//如果重定向失败，直接标记退出码为 1，不再执行 builtin
	if (shell->cmds->redirs && exec_redir(shell->cmds) == -1)
        shell->exit_status = 1;
    //没有重定向，或者重定向成功，就正常执行 builtin
	//exec_builtin 内部写的东西，此时会流向重定向指定的文件
	else
		shell->exit_status = exec_builtin(shell);
	//不管上面走了哪条路径，执行完都要把 fd 改回来
	//用备份的 fd 覆盖回 STDOUT_FILENO / STDIN_FILENO
	//相当于把水管重新接回终端
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	//备份用的 fd 已经用完了，及时关闭，避免 fd 泄漏
	close(saved_stdout);
	close(saved_stdin);
}
//解决单命令问题
void	exec_single(t_shell *shell)
{
	
	pid_t	pid;
	int		status;
	//如果av[0]是builtin则执行builtin函数 不是则执行常规函数
	if(is_builtin(shell->cmds))
	{
		exec_builtin_with_redir(shell);
		return ;//return (exec_external(cmds,env));
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
	waitpid(pid, &status, 0);
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
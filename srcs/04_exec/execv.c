/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execv.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 00:22:53 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/06 00:57:53 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
遍历每个 cmd
        ↓
处理 heredoc
        ↓
处理 redir
        ↓
builtin ?
        ↓
有 / ?
        ↓
PATH查找
        ↓
fork
        ↓
execve*/
/*
executor()
│
├── count_cmds()
│   └── 数有几个 cmd
│
├── 如果只有 1 个 cmd
│   └── exec_single()
│       │
│       ├── 如果是 builtin
│       │   └── exec_builtin()        // 父进程执行 cd/export/unset/exit
│       │
│       └── 如果是 external
│           ├── fork()
│           ├── 子进程：exec_cmd()
│           └── 父进程：waitpid()
│
└── 如果多个 cmd
    └── exec_pipe()
        │
        ├── create_pipes()
        │   └── 创建 pipes[n - 1][2]
        │
        ├── fork 每个 cmd
        │   │
        │   └── 子进程：
        │       ├── setup_pipe()
        │       │   └── dup2 输入/输出
        │       │
        │       ├── close_all_pipes()
        │       │   └── 关闭所有不用的 fd
        │       │
        │       ├── apply_redirs()    // 之后写
        │       │
        │       └── exec_cmd()
        │
        ├── 父进程：
        │   ├── close_all_pipes()
        │   └── wait 所有子进程
        │
        └── 更新 g_exit_status
		*/
	/*
	
					┌─────────────────┐
                    │   执行入口判断    │
                    │ (根据是否有 | )   │
                    └────────┬─────────┘
                             │
              ┌──────────────┴──────────────┐
              │                              │
       单个命令                          多个命令(有|)
              │                              │
       exec_single()                   exec_pipe()
              │                              │
    ┌─────────┴─────────┐         ┌──────────┴──────────┐
    │ 1.判断is_builtin   │         │ 1.count_cmds算n      │
    │   是→父进程直接执行 │         │ 2.开n-1个管道         │
    │   (cd/export/exit) │         │ 3.循环fork n个子进程   │
    │ 2.不是→fork子进程   │         │ 4.每个子进程里：       │
    │ 3.子进程调exec_cmd  │         │    - dup2接管道       │
    │ 4.父进程waitpid    │         │    - 关所有管道fd      │
    └────────────────────┘         │    - 调exec_cmd       │
                                    │ 5.父进程关所有管道fd   │
                                    │ 6.父进程waitpid全部    │
                                    └───────────┬───────────┘
                                                │
                                                ▼
                                         exec_cmd()
                                    ┌───────────────────────┐
                                    │ (只在子进程里跑)         │
                                    │ 1.判断is_builtin        │
                                    │   是→exec_builtin+exit  │
                                    │ 2.不是→get_path         │
                                    │ 3.execve                │
                                    │ 4.失败→perror+exit(127) │
                                    └─────────────────────────┘
	
	*/
int	executor(t_shell *shell, t_cmd *cmds, t_env *env)
{
	int	n_cmds;

	n_cmds = count_cmds(cmds);
	if (!cmds)
		return (1);
	if (n_cmds == 1)
		exec_single(shell,cmds, env);
	else
		exec_pipe(shell, cmds, env);
	return (shell->exit_status);
	
}
/*04_exec
├── executor.c        # 执行入口：判断单命令 / 多管道
├── exec_cmd.c        # 执行一个 command
├── exec_path.c       # 找 PATH，拼接可执行路径
├── exec_pipe.c       # 多个 pipe 的创建、fork、dup2
├── exec_redir.c      # 执行重定向 < > >>
├── exec_heredoc.c    # heredoc <<
├── exec_utils.c      # exec 辅助函数
└── exec_error.c      # exec 错误打印*/

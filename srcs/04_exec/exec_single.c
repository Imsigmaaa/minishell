/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:12:35 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:21:45 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	saved_stdio(int	*saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout == -1 || *saved_stdin == -1)
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
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
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
	if (!shell->cmds->argv || !shell->cmds->argv[0])
	{
		shell->exit_status = 0;//重定向成功，但没有命令，所以状态为 0
		return ;//返回
	}
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

void	exec_single(t_shell *shell)
{
	
	pid_t	pid;
	int		status;
	if (!shell->cmds->argv || !shell->cmds->argv[0] || is_builtin(shell->cmds))
	{
		exec_in_parent(shell);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
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
	update_exit_status(shell, status);
}

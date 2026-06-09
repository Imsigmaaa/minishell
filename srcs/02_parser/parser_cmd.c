/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:34:51 by xingchen          #+#    #+#             */
/*   Updated: 2026/06/09 17:31:39 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <string.h>
#include "../../libft/libft.h"
//声明新的cmd 节点
t_cmd	*new_cmd(void)
{
	t_cmd *cmd;

	cmd = malloc(sizeof(t_cmd));
	if(!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}
//安全释放整个cmds
void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		ft_free_arr(cmds->argv);
		ft_free_t_redir(cmds->redirs);
		free(cmds);
		cmds = tmp;
	}
}
//追加cmd 因为pipe标志着需要创建新cmd
int	add_arg(t_cmd *cmd, t_token *token)
{
	char **tmp;
	char *value;
	size_t	size;
	size_t	i;
	
	//malloc一个新地址 防止后面double free
	value = ft_strdup(token->value);
	if(!value)
		return(perror("malloc"), 0);
	//计算旧array的大小
	size = ft_arrlen(cmd->argv);
	//malloc一个新的array 每次多一个字符串 +2是因为NULL需要一块空间
	tmp = malloc(sizeof(char *) * (size + 2));
	if(!tmp)
		return(free(value),perror("malloc"), 0) ;
	i = 0;
	while (i < size)
	{	
		tmp[i] = cmd->argv[i];
		i ++;
	}
	tmp[i++] = value;
	tmp[i] = NULL;
	free(cmd->argv);
	cmd->argv = tmp;
	return(1);
}
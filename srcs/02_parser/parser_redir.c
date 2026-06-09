/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:40:49 by xingchen          #+#    #+#             */
/*   Updated: 2026/06/09 17:31:34 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../libft/libft.h"
#include <string.h>

//声明一个新重定向节点并且初始化为NULL；（declarer un nouveau node de redir et init à NULL）
t_redir	*new_redir(void)
{
	t_redir	*redir;
	
	redir = malloc(sizeof(t_redir));
	if(!redir)
		return NULL;
	redir->target = NULL;
	redir->next = NULL;
	return (redir);
}
// free 重定向链表（free la list de redirs）
void	ft_free_t_redir(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs->next;
		free(redirs->target);
		free(redirs);
		redirs = tmp;
	}
}
//追加重定向节点（ajouter un node de redir）
/*为什么 追加重定向节点 比如 echo "hello" < infile1 < infile2 这是2个节点 一个命令可以多个重定向
pourquoi ajoute le node de redir, 
ex:echo "hello" < infile1 < infile2 , 
une commande peut avoir plusiers redirections
*/
int	add_redir(t_cmd *cmd, t_token *tokens)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = new_redir();
	
	if(!redir)
		return(perror("malloc"), 0);//如果malloc失败打印malloc错误
	//如果开始是NULL说明这是第一个节点（si cmd->redirs est NULL, c'est à dire redire est le premier node）
	if(!cmd->redirs)
		cmd->redirs = redir;
	else//如果不是NULL则是追加节点（si non, on ajoute un node）
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
	redir->type = tokens->type;//将这个类型复制到重定向里
	//重定向符号的指向的下一个节点就是他的目标文件，并且重新mallo一块地址，方便tokens和redir分开释放防止double free
	redir->target = ft_strdup(tokens->next->value);
	if(!redir->target)
		return(perror("malloc"), 0);
	return (1);
}
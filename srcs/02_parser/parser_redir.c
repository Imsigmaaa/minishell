/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:40:49 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 16:36:19 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC)
}

t_redir	*new_redir(void)
{
	t_redir	*redir;
	
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return NULL;
	redir->target = NULL;
	redir->next = NULL;
	return (redir);
}

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

int	add_redir(t_cmd *cmd, t_token *tokens)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = new_redir();
	if (!redir)
		return(perror("malloc"), 0);//如果malloc失败打印malloc错误
	redir->type = tokens->type;//将这个类型复制到重定向里
	redir->target = ft_strdup(tokens->next->value);
	if (!redir->target)
		return (perror("malloc"), 0);
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
	return (1);
}
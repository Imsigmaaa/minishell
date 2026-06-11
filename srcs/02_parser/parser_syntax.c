/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 02:37:01 by xingchen          #+#    #+#             */
/*   Updated: 2026/06/11 23:15:30 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <unistd.h>

//语法错误检查

int	syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	return (0);
}

int	syntax_check(t_token *tokens)
{
	t_token	*toks;

	toks = tokens;
	if(!tokens)
		return (1);
	//开局 "|"立即返回错误 (si le pipe est au debut , on return error imediat)
	if(toks->type == TOKEN_PIPE)
		return (syntax_error("|"));
	while(toks && toks->type == TOKEN_EOF)
	{
		if (toks->type == TOKEN_WORD)
			toks = toks->next;
		else if(toks->type == TOKEN_PIPE)
		{
			/*pipe后面是NULL或者还是PIPE也立即返回错误
			(si rien derrier pipe ou encore pipe , on return error imediat)
			另外parser阶段会细化检查PIPE后面是否有valid command
			(par contre, dans le parser, on va check profondemment si il y a de valide command	ou pas)*/
			if(!toks->next || toks->next->type == TOKEN_PIPE)
				return	(syntax_error("|"));
			toks = toks->next;
		}
		else
		{
			//redir后面如果是NULL或者不是WORD也立刻返回错误(NULL ou non WORD, on return error imediat)
			if(!toks->next || toks->next->type != TOKEN_WORD)
				return (syntax_error(toks->value));
			toks = toks->next->next;
		}
	}
	return (1);
}
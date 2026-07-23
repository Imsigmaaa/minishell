/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 21:38:26 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/23 17:05:17 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"

#include "minishell.h"

int	process_token(t_cmd **cur_cmd, t_token **cur_tok)
{
	if ((*cur_tok)->type == TOKEN_WORD)
	{
		if (!add_arg(*cur_cmd, *cur_tok))
			return (0);
	}
	else if ((*cur_tok)->type == TOKEN_PIPE)
	{
		(*cur_cmd)->next = new_cmd();
		if (!(*cur_cmd)->next)
			return (perror("malloc"), 0);
		*cur_cmd = (*cur_cmd)->next;
	}
	else if (is_redir_token((*cur_tok)->type))
	{
		if (!add_redir(*cur_cmd, *cur_tok))
			return (0);
		*cur_tok = (*cur_tok)->next;
	}
	else
		return (0);
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cur_cmd;
	t_token	*cur_tok;
	
	if (!tokens || tokens->type == TOKEN_EOF)
		return (NULL);
	cur_tok = tokens;
	cur_cmd = new_cmd();
	if (!cur_cmd)
		return (perror("malloc"), NULL);
	cmds = cur_cmd;
	while (cur_tok && cur_tok->type != TOKEN_EOF)
	{
		if (!process_token(&cur_cmd, &cur_tok))
			return (free_cmds(cmds), NULL);
		cur_tok = cur_tok->next;
	}
	return (cmds);
}

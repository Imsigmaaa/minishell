/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 22:30:59 by yushan            #+#    #+#             */
/*   Updated: 2026/06/11 22:13:31 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')' || c == ';');
}

t_token	*lexer(char *input, int *err) //接入主程序的processe line部分
{
	t_lexer	lex;
	int		i;

	lex.head = NULL;
	lex.tail = NULL;
	lex.err = LEX_OK;
	i = 0;
	while (input && input[i] && lex.err == LEX_OK)
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_operator_char(input[i]))
			add_operator_token(&lex, input, &i);
		else
			add_word_token(&lex, input, &i);
	}
	if (lex.err == LEX_OK && !add_token(&lex, TOKEN_EOF, NULL, 0, 0))
		free_tokens(&lex.head);
	if (lex.err != LEX_OK)
		free_tokens(&lex.head);
	*err = lex.err;
	return (lex.head);
}

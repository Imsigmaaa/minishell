/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 22:30:47 by yushan            #+#    #+#             */
/*   Updated: 2026/06/10 00:29:22 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_lexer_error(int err)
{
	if (err == LEX_ERR_UNCLOSED_QUOTE)
		write(2, "minishell: syntax error: unclosed quote\n", 40);
	else if (err == LEX_ERR_UNSUPPORTED_TOKEN)
		write(2, "minishell: syntax error: unsupported token\n", 43);
	else if (err == LEX_ERR_MALLOC)
		write(2, "minishell: malloc error\n", 24);
}

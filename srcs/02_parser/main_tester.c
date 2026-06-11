/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tester.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:11:13 by xingchen          #+#    #+#             */
/*   Updated: 2026/06/11 21:53:57 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cmds(t_cmd *cmds)
{
	t_cmd *tmp;

	tmp =  cmds;
	while (tmp)
	{
		int i = 0;
		while (tmp->argv &&tmp->argv[i])
		{
			printf("%s\n",tmp->argv[i]);
			i ++;
		}
		t_redir *redir = tmp->redirs;
		while (redir)
		{
			printf("%d\n",redir->type);
			printf("%s\n",redir->target);
			redir = redir->next;	
		}
		tmp = tmp->next;	
	}
}
int main(void)
{
	t_token t1;
	t_token t2;
	t_token t3;
	t_token t4;
	t_token t5;
	t_cmd	*cmds;	
	t1.type = TOKEN_PIPE;
	t1.value = "|";
	t1.next = &t2;
	
	t2.type = TOKEN_WORD;
	t2.value = "\"hello\"";
	t2.next = &t3;

	
	
	t3.type = TOKEN_REDIR_OUT;
	t3.value = ">";
	t3.next = &t4;

	t4.type = TOKEN_WORD;
	t4.value = "file";
	t4.next = &t5;

	t5.type = TOKEN_PIPE;
	t5.value = "|";
	t5.next = NULL;

	if(!syntax_check(&t1))
		return 0;
	cmds = parse_tokens(&t1);

	print_cmds(cmds);
	free_cmds(cmds);

}
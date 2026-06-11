/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:07:58 by yushan            #+#    #+#             */
/*   Updated: 2026/06/11 22:09:14 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
# include "../libft/libft.h"

/*ENV*/
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

/*
原始输入
→ Lexer 生成 token，并保留 quote context
→ Parser 根据 PIPE、&&、||、括号建立 AST
→ Syntax validation 检查结构是否合法
→ Expansion 遍历 AST 的 SIMPLE_CMD 节点
→ 把 word 展开成最终参数
→ 把 redirection target 展开成最终文件名
→ 把 word list 转换成 char **argv
→ Executor 递归执行 AST*/

/*TOKEN*/

typedef enum e_token_type
{
    TOKEN_WORD, // 普通 word，可能是命令名、参数、文件名、heredoc delimiter
    TOKEN_PIPE,
    TOKEN_REDIR_IN, //输入重定向
    TOKEN_REDIR_OUT, //输出重定向，覆盖
    TOKEN_APPEND, //输出重定向，追加
    TOKEN_HEREDOC, //heredoc
    TOKEN_AND, //bonus 逻辑 AND
    TOKEN_OR,
    TOKEN_LPAREN, //bonus 左括号，用于 AST 优先级
    TOKEN_RPAREN, //bonus 右括号
    TOKEN_EOF
}   t_token_type;

typedef struct s_token
{
    char                *value;
    t_token_type        type;
    int                 has_quote;
    int                 has_wildcard;
    struct s_token      *next;
}   t_token;

typedef enum e_lex_error
{
	LEX_OK, //0
	LEX_ERR_MALLOC, //1
	LEX_ERR_UNCLOSED_QUOTE, //2
	LEX_ERR_UNSUPPORTED_TOKEN //3
}	t_lex_error;

typedef struct s_lexer
{
	t_token	*head;
	t_token	*tail;
	int		err;
}	t_lexer;

typedef struct s_redir
{
    t_token_type        type;
    char                *target;
    struct s_redir      *next;
}   t_redir;

typedef struct s_cmd
{
    char            **argv;
    t_redir         *redirs;
    struct s_cmd    *next;
}   t_cmd;

typedef struct s_shell
{
    t_env       *env;
    t_token     *tokens;
    t_cmd       *commands;
    int         exit_status; //保存 $? 的值 : 最近一次执行完成的前台命令或前台 pipeline 的退出状态
    int         should_exit; // 让 exit builtin 结束主循环
}   t_shell;

void	print_lexer_error(int err);
int		add_word_token(t_lexer *lex, char *s, int *i);
int		add_operator_token(t_lexer *lex, char *s, int *i);
int		add_token(t_lexer *lex, t_token_type type, char *value,
		int has_quote, int has_wildcard);
int		is_space(char c);
int		is_operator_char(char c);
char	*ms_strdup(char *s);
char	*ms_substr(char *s, int start, int len);
void	free_tokens(t_token **tokens);
t_token *lexer(char *input, int *err);

t_cmd	*parse_tokens(t_token *token);
t_cmd	*new_cmd(void);
void	free_cmds(t_cmd *cmds);
//void	free_tokens(t_token *tokens);
int	add_arg(t_cmd *cmd, t_token *token);
int	add_redir(t_cmd *cmd, t_token *tokens);
size_t	ft_arrlen(char **arr);
void	ft_free_arr(char **arr);
void	ft_free_t_redir(t_redir *redirs);
t_token *tokenize(char *line);
int	syntax_check(t_token *tokens);
int	syntax_error(char *token);

#endif


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yushan <yushan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:07:58 by yushan            #+#    #+#             */
/*   Updated: 2026/06/04 14:40:41 by yushan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

typedef struct s_shell
{
    t_env       *env;
    t_token     *tokens;
    t_cmd       *commands;
    int         exit_status; //保存 $? 的值 : 最近一次执行完成的前台命令或前台 pipeline 的退出状态
    int         should_exit; // 让 exit builtin 结束主循环
}   t_shell;

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
/*Lexer 必须保留 Quote Context*/
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC
}   t_token_type;

typedef struct s_token
{
    char                *value;
    t_token_type        type;
    struct s_token      *next;
}   t_token;

/*commande*/
/*Parser 应该先建立 AST，再由 Expansion 处理叶子节点*/
/* expansion要处理
$VAR
$?
quote removal
bonus wildcard **/
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

#endif

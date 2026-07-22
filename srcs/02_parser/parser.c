/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 21:38:26 by xingchen          #+#    #+#             */
/*   Updated: 2026/07/08 18:01:34 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"

#include "minishell.h"


/*static	int has_valid_command_after_pipe(t_token *tokens)
{
	t_token	*tmp;
	//此函数留到EXECUTOR阶段
	tmp = tokens->next;
	if(!tmp)
		return (0);
	while (tmp)
	{
		if(tmp->type == TOKEN_PIPE)
			return (0);
		else if(tmp->type == TOKEN_WORD)
			return (1);
		else
		{
			tmp = tmp->next;
			if(!tmp || tmp->type != TOKEN_WORD)
				return (0);
			tmp = tmp->next;
		}
	}
	return (0);
}*/

int	process_token(t_cmd **cur_cmd, t_token **cur_tok)
{
	//如果是WORD类型
	if((*cur_tok)->type == TOKEN_WORD)
	{
		/*在cmd->argv里面扩容argv,
		比如之前cmd->argv=｛“echo”,NULL｝,
		后面遇见“hello”需要追加字符串变成｛“echo”, "\"hello\"",NULL｝以此类推*/
		//如果追加失败返回0
		if(!add_arg(*cur_cmd, *cur_tok))
			return (0);
	}
	//如果是PIPE类型
	else if((*cur_tok)->type == TOKEN_PIPE)
	{
		(*cur_cmd)->next = new_cmd();
		if(!(*cur_cmd)->next)
			return(perror("malloc"), 0);
		*cur_cmd = (*cur_cmd)->next;
	}
	/*如果是REDIR类型 
	标准格式是一个重定向符号 后面跟一个文件名即WORD类型，
	如果当前类型是REDIR ，但是token->next->type不是WORD类型
	说明是语法错误，但是此检测已经在syntax_check函数中检查过了。
	所以REDIR后面肯定是WORD类型，我们这里直接add就可以，如果add失败则返回0*/
	else
	{
		if(!add_redir(*cur_cmd, *cur_tok))
			return (0);
		*cur_tok = (*cur_tok)->next;
	}
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cur_cmd;
	t_token	*cur_tok;
	
	if(!tokens)
		return (NULL);
	//声明一个cur_tok 用它来参与cmd链表的建立
	cur_tok = tokens;
	//声明一个新的cmd节点 new_cmd函数创建节点并且初始化内容为NULL
	cur_cmd = new_cmd();
	//如果返回值为null说明malloc失败 要返回因为malloc失败的错误 用perror
	if(!cur_cmd)
		return(perror("malloc"), NULL);
	//cmds链表指向第一个节点
	cmds = cur_cmd;
	//开始循环tokens
	while (cur_tok && cur_tok->type != TOKEN_EOF)
	{
		//用process_token函数处理当前token, 用&为了改变外面的指针，如果返回值位0则释放并返回
		if(!process_token(&cur_cmd, &cur_tok))
			return(free_cmds(cmds), NULL);
		cur_tok = cur_tok->next;
	}
	return (cmds);
}
/*

parser 你可以理解成：

lexer 只是把字符串切成 token，parser 是把 token 变成“命令结构体”。

比如：

cat < in.txt | grep hello > out.txt

lexer 得到：

WORD(cat)
IN(<)
WORD(in.txt)
PIPE(|)
WORD(grep)
WORD(hello)
OUT(>)
WORD(out.txt)

parser 要把它变成：

cmd1:
  argv = ["cat"]
  redir = < in.txt

cmd2:
  argv = ["grep", "hello"]
  redir = > out.txt

中间用 pipe 连接。

1. parser 最终要生成什么？

建议你先定义一个命令结构体：

typedef struct s_redir
{
	int				type;      // T_IN / T_OUT / T_APPEND / T_HEREDOC
	char			*file;     // 文件名 或 heredoc delimiter
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;    // 命令参数
	t_redir			*redirs;   // 重定向链表
	struct s_cmd	*next;     // pipe 后面的命令
}	t_cmd;

也就是说 parser 最后生成的是：

t_cmd *cmds;

它是一个链表：

cmd1 -> cmd2 -> cmd3 -> NULL

每个 cmd 里面有自己的：

argv
redirs
2. parser 的大步骤

parser 大概分 4 步：

1. 检查 token 语法对不对
2. 遇到 | 就分成一条新命令
3. 遇到 < > >> << 就保存成 redirection
4. 普通 WORD 就保存进 argv
3. 先做语法检查

parser 第一件事不是马上建结构体，而是检查有没有明显错误。

比如这些都是错误：

| ls
ls |
ls || wc
ls > 
ls < | wc
ls > > file

规则很简单：

pipe 的规则

| 不能在开头：

| ls

| 不能在结尾：

ls |

| 后面不能马上还是 |：

ls || wc
redirection 的规则

重定向后面必须跟一个 WORD：

ls >
ls < |
ls > > file

也就是说：

if token is < > >> <<
	next token 必须是 WORD

否则 syntax error。

4. parser 主循环怎么写？

逻辑可以很白话：

从第一个 token 开始看

创建一个 cmd

如果看到 WORD：
    加到当前 cmd 的 argv

如果看到 < > >> <<：
    下一个 token 是文件名
    创建 redir
    加到当前 cmd 的 redirs
    跳过这两个 token

如果看到 |：
    当前 cmd 结束
    创建下一个 cmd
    继续解析



5. 举例：echo hello | wc -l

tokens：

WORD echo
WORD hello
PIPE
WORD wc
WORD -l

parser 过程：

创建 cmd1

看到 echo
加入 cmd1 argv

看到 hello
加入 cmd1 argv

看到 |
cmd1 结束
创建 cmd2

看到 wc
加入 cmd2 argv

看到 -l
加入 cmd2 argv

最后：

cmd1:
  argv = ["echo", "hello", NULL]
  redirs = NULL
  next = cmd2

cmd2:
  argv = ["wc", "-l", NULL]
  redirs = NULL
  next = NULL
6. 举例：cat < in.txt > out.txt

tokens：

WORD cat
IN <
WORD in.txt
OUT >
WORD out.txt

parser 过程：

创建 cmd1

cat 是 WORD
加入 argv

< 是 redir
下一个 in.txt 是文件名
保存 redir: type = T_IN, file = "in.txt"

> 是 redir
下一个 out.txt 是文件名
保存 redir: type = T_OUT, file = "out.txt"

最后：

cmd1:
  argv = ["cat", NULL]

  redirs:
    < in.txt
    > out.txt
7. parser 不要做什么？

parser 阶段不要执行命令。

不要 open 文件。

不要 fork。

不要 pipe。

不要 expand $USER。

不要删除引号。

parser 只负责：

把 token 整理成命令结构体

这些留给后面：

expansion 处理 $VAR 和引号
exec 处理执行、pipe、redir
8. parser 文件可以这样分
parser/
├── parser.c       // 主入口 parse_tokens
├── syntax.c       // 检查语法错误
├── cmd.c          // new_cmd, add_arg
└── redir.c        // new_redir, add_redir
9. parser 最小实现顺序

建议你按这个顺序写：

第一步：写结构体
t_cmd
t_redir
第二步：写判断函数
int	is_redir(t_token_type type);
int	is_word(t_token_type type);
第三步：写 syntax check

先支持这些错误：

| ls
ls |
ls || wc
ls >
ls < |
ls > > file
第四步：写 new_cmd
t_cmd *new_cmd(void);

里面初始化：

argv = NULL;
redirs = NULL;
next = NULL;
第五步：写 add_arg

把一个 WORD 加进：

cmd->argv

例如：

echo hello

变成：

argv[0] = "echo";
argv[1] = "hello";
argv[2] = NULL;
第六步：写 add_redir

把：

> out.txt

变成：

redir->type = T_OUT;
redir->file = "out.txt";

然后挂到：

cmd->redirs
第七步：写 parse_tokens

把前面所有函数串起来。

10. 最核心一句话

lexer：

把输入字符串切成 token

parser：

把 token 分组，变成 cmd 链表

executor：

拿 cmd 链表去执行

parser 看到：

echo hello > out.txt | wc -l

最后应该得到：

cmd1:
  argv: echo hello
  redir: > out.txt

cmd2:
  argv: wc -l

这就是 parser 的核心工作。*/

int	process_token(t_cmd **cur_cmd, t_token **cur_tok)
{
	if((*cur_tok)->type == TOKEN_WORD)
	{
		if(!add_arg(*cur_cmd, *cur_tok))
			return (0);
	}
	else if((*cur_tok)->type == TOKEN_PIPE)
	{
		(*cur_cmd)->next = new_cmd();
		if(!(*cur_cmd)->next)
			return(perror("malloc"), 0);
		*cur_cmd = (*cur_cmd)->next;
	}
	else
	{
		if(!add_redir(*cur_cmd, *cur_tok))
			return (0);
		*cur_tok = (*cur_tok)->next;
	}
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cur_cmd;
	t_token	*cur_tok;
	
	if(!tokens)
		return (NULL);
	cur_tok = tokens;
	cur_cmd = new_cmd();
	if(!cur_cmd)
		return(perror("malloc"), NULL);
	cmds = cur_cmd;
	while (cur_tok && cur_tok->type != TOKEN_EOF)
	{
		if(!process_token(&cur_cmd, &cur_tok))
			return(free_cmds(cmds), NULL);
		cur_tok = cur_tok->next;
	}
	return (cmds);
}
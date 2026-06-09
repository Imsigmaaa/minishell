目标

输入：

echo "hello world" | grep hello > out.txt

输出：

WORD     -> echo
WORD     -> "hello world"
PIPE     -> |
WORD     -> grep
WORD     -> hello
OUT      -> >
WORD     -> out.txt

Lexer的工作只有一句话：

把字符串切成 Token 链表。

不负责：

expansion
parser
exec

第一步：主函数
t_token *lexer(char *input);

作用：

echo hello | cat

从左往右扫描整个字符串。

伪代码：

while (input[i])
{
    跳过空格

    判断当前字符是什么

    创建token

    加到链表
}
第二步：跳过空格

单独函数：

void skip_spaces(char *str, int *i);

作用：

例如：

echo      hello

中间多个空格。

把：

*i

移动到：

h

的位置。

第三步：判断特殊符号
t_token *read_operator(char *str, int *i);

负责：

|
<
>
<<
>>

例如：

当前：

>>
 ^

返回：

T_APPEND

并且：

i += 2;
第四步：读取普通单词

这是最核心的。

t_token *read_word(char *str, int *i);


负责读取：

echo
hello
$USER
abc.txt

例如：

echo hello
     ^

一直读到：

空格
|
<
>
字符串结束

为止。

得到：

hello

第五步：处理引号

建议单独函数。

char *extract_word(char *str, int *i);

或者：

char *read_quoted_part(...)

例如：

echo "hello world"

当前：

     ^

发现：

"

就一直读到下一个：

"

不要把里面的空格当分隔符。

最终得到：

"hello world"

整个保存。

再比如：

echo 'abc $USER'

得到：

'abc $USER'

原样保存。
为什么 Lexer 不展开变量

例如：

echo "$USER"

Lexer阶段：

保存：

"$USER"

Expansion阶段：

再变成：

"chen"

所以：

Lexer只保存。

不解释。
第六步：创建Token

单独函数：

t_token *new_token(t_token_type type, char *value);

作用：

WORD -> echo

变成：

node->type = T_WORD;
node->value = "echo";

第七步：加入链表

void token_add_back(t_token **lst, t_token *new);

作用：

echo hello

生成：

[echo] -> [hello]

推荐最终结构

lexer/
├── lexer.c
├── lexer_utils.c
├── token.c
├── quote.c

lexer.c

主循环

lexer()

lexer_utils.c

skip_spaces()

read_word()

read_operator()

quote.c

read_single_quote()

read_double_quote()

extract_word()

token.c

new_token()

token_add_back()

token_last()

整体流程

输入：

echo "hello world" | cat > out

主循环：

e
↓
read_word
↓
WORD echo

"
↓
extract_word
↓
WORD "hello world"

|
↓
read_operator
↓
PIPE

cat
↓
WORD cat

>
↓
OUT

out
↓
WORD out

最终：

[echo]
   ↓
["hello world"]
   ↓
[|]
   ↓
[cat]
   ↓
[>]
   ↓
[out]

这就是一个完整的 Minishell Lexer 的基本框架。

对于 42 的 Minishell，其实还可以再细分成：

    token结构体定义

    token链表管理

    operator识别

    word提取

    quote处理



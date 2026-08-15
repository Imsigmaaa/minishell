我会按 minishell 的真实执行顺序讲：先说明 expansion 是干嘛的，再拆成函数和步骤，最后用几个命令例子串起来。

Thought for a couple of seconds

minishell 的 expansion，白话说就是：

把用户输入里的变量、引号处理成真正要执行的字符串。

比如用户输入：

echo "$USER"

lexer/parser 阶段可能只是保存：

echo
"$USER"

到了 expansion 阶段，要变成：

echo
chen
一、expansion 在 minishell 里的位置

整体顺序大概是：

readline
   ↓
lexer 分词
   ↓
parser 组成 command
   ↓
expansion 展开变量 / 处理引号
   ↓
executor 执行

也就是说：

lexer 不展开变量，parser 也不展开变量。

真正处理 $USER、$?、引号删除，是 expansion 做。

二、expansion 主要做 3 件事
1. 变量展开

处理这种东西：

$USER
$HOME
$?

例如：

echo $HOME

变成：

echo /home/chen

$? 是上一条命令的退出状态：

echo $?

如果上一条成功，就是：

0
2. 区分单引号和双引号

规则非常重要：

单引号 '...'

里面什么都不展开。

echo '$USER'

输出：

$USER

所以 expansion 遇到单引号里面的 $，不要处理。

双引号 "..."

里面的变量要展开。

echo "$USER"

输出：

chen

所以 expansion 遇到双引号里面的 $USER，要展开。

3. 删除引号

执行前，引号本身不应该传给程序。

比如：

echo "hello world"

最终传给 execve 的参数应该是：

argv[0] = "echo";
argv[1] = "hello world";

不是：

argv[1] = "\"hello world\"";

所以 expansion 最后要把引号去掉。

三、建议你把 expansion 分成几个函数

可以这样分：

char    *expand_word(char *word, t_shell *shell);
char    *expand_var(char *word, int *i, t_shell *shell);
char    *get_var_name(char *word, int *i);
char    *get_env_value(char *name, t_env *env);
char    *remove_quotes(char *word);
四、一步一步做
第 1 步：遍历每个 command 的 argv

你的 parser 最后大概会得到：

cmd->argv = {"echo", "$USER", NULL};

expansion 要对每一个参数做处理：

i = 0;
while (cmd->argv[i])
{
    cmd->argv[i] = expand_word(cmd->argv[i], shell);
    i++;
}
第 2 步：写 expand_word

它负责处理一个字符串。

比如：

"$USER hello"

你要一个字符一个字符看：

遇到普通字符 → 复制
遇到单引号 → 进入单引号模式
遇到双引号 → 进入双引号模式
遇到 $ → 看当前是不是允许展开

核心逻辑：

如果在单引号里：
    原样复制，不展开 $

如果不在单引号里：
    遇到 $USER 就展开
第 3 步：处理 $

几种情况：

$USER
$HOME
$?
$

规则：

$?      → shell->exit_status
$USER   → 去 env 里面找 USER
$HOME   → 去 env 里面找 HOME
$不存在 → 变成空字符串
单独 $  → 保留 $

例子：

echo $ABC

如果环境变量里没有 ABC，最后相当于：

echo
第 4 步：删除引号

比如原字符串：

"hello world"

expansion 后应该是：

hello world

单引号也是：

'hello'

变成：

hello

但是注意：

'$USER'

变成：

$USER

不是 chen。

五、最重要的例子
例子 1

输入：

echo $USER

lexer/parser：

argv[0] = "echo";我会按 minishell 的真实执行顺序讲：先说明 expansion 是干嘛的，再拆成函数和步骤，最后用几个命令例子串起来。

minishell 的 expansion，白话说就是：

把用户输入里的变量、引号处理成真正要执行的字符串。

比如用户输入：

echo "$USER"

lexer/parser 阶段可能只是保存：

echo
"$USER"

到了 expansion 阶段，要变成：

echo
chen

一、expansion 在 minishell 里的位置

整体顺序大概是：

readline
   ↓
lexer 分词
   ↓
parser 组成 command
   ↓
expansion 展开变量 / 处理引号
   ↓
executor 执行

也就是说：

lexer 不展开变量，parser 也不展开变量。

真正处理 $USER、$?、引号删除，是 expansion 做。
二、expansion 主要做 3 件事
1. 变量展开

处理这种东西：

$USER
$HOME
$?

例如：

echo $HOME

变成：

echo /home/chen

$? 是上一条命令的退出状态：

echo $?

如果上一条成功，就是：

0

2. 区分单引号和双引号

规则非常重要：
单引号 '...'

里面什么都不展开。

echo '$USER'

输出：

$USER

所以 expansion 遇到单引号里面的 $，不要处理。
双引号 "..."

里面的变量要展开。

echo "$USER"

输出：

chen

所以 expansion 遇到双引号里面的 $USER，要展开。
3. 删除引号

执行前，引号本身不应该传给程序。

比如：

echo "hello world"

最终传给 execve 的参数应该是：

argv[0] = "echo";
argv[1] = "hello world";

不是：

argv[1] = "\"hello world\"";

所以 expansion 最后要把引号去掉。
三、建议你把 expansion 分成几个函数

可以这样分：

char    *expand_word(char *word, t_shell *shell);
char    *expand_var(char *word, int *i, t_shell *shell);
char    *get_var_name(char *word, int *i);
char    *get_env_value(char *name, t_env *env);
char    *remove_quotes(char *word);

四、一步一步做
第 1 步：遍历每个 command 的 argv

你的 parser 最后大概会得到：

cmd->argv = {"echo", "$USER", NULL};

expansion 要对每一个参数做处理：

i = 0;
while (cmd->argv[i])
{
    cmd->argv[i] = expand_word(cmd->argv[i], shell);
    i++;
}

第 2 步：写 expand_word

它负责处理一个字符串。

比如：

"$USER hello"

你要一个字符一个字符看：

遇到普通字符 → 复制
遇到单引号 → 进入单引号模式
遇到双引号 → 进入双引号模式
遇到 $ → 看当前是不是允许展开

核心逻辑：

如果在单引号里：
    原样复制，不展开 $

如果不在单引号里：
    遇到 $USER 就展开

第 3 步：处理 $

几种情况：

$USER
$HOME
$?
$

规则：

$?      → shell->exit_status
$USER   → 去 env 里面找 USER
$HOME   → 去 env 里面找 HOME
$不存在 → 变成空字符串
单独 $  → 保留 $

例子：

echo $ABC

如果环境变量里没有 ABC，最后相当于：

echo

第 4 步：删除引号

比如原字符串：

"hello world"

expansion 后应该是：

hello world

单引号也是：

'hello'

变成：

hello

但是注意：

'$USER'

变成：

$USER

不是 chen。
五、最重要的例子
例子 1

输入：

echo $USER

lexer/parser：

argv[0] = "echo";
argv[1] = "$USER";

expansion 后：

argv[1] = "chen";

例子 2

输入：

echo '$USER'

单引号不展开。

expansion 后：

argv[1] = "$USER";我会按 minishell 的真实执行顺序讲：先说明 expansion 是干嘛的，再拆成函数和步骤，最后用几个命令例子串起来。

minishell 的 expansion，白话说就是：

把用户输入里的变量、引号处理成真正要执行的字符串。

比如用户输入：

echo "$USER"

lexer/parser 阶段可能只是保存：

echo
"$USER"

到了 expansion 阶段，要变成：

echo
chen

一、expansion 在 minishell 里的位置

整体顺序大概是：

readline
   ↓
lexer 分词
   ↓
parser 组成 command
   ↓
expansion 展开变量 / 处理引号
   ↓
executor 执行

也就是说：

lexer 不展开变量，parser 也不展开变量。

真正处理 $USER、$?、引号删除，是 expansion 做。
二、expansion 主要做 3 件事
1. 变量展开

处理这种东西：

$USER
$HOME
$?

例如：

echo $HOME

变成：

echo /home/chen

$? 是上一条命令的退出状态：

echo $?

如果上一条成功，就是：

0

2. 区分单引号和双引号

规则非常重要：
单引号 '...'

里面什么都不展开。

echo '$USER'

输出：

$USER

所以 expansion 遇到单引号里面的 $，不要处理。
双引号 "..."

里面的变量要展开。

echo "$USER"

输出：

chen

所以 expansion 遇到双引号里面的 $USER，要展开。
3. 删除引号

执行前，引号本身不应该传给程序。

比如：

echo "hello world"

最终传给 execve 的参数应该是：

argv[0] = "echo";
argv[1] = "hello world";

不是：

argv[1] = "\"hello world\"";

所以 expansion 最后要把引号去掉。
三、建议你把 expansion 分成几个函数

可以这样分：

char    *expand_word(char *word, t_shell *shell);
char    *expand_var(char *word, int *i, t_shell *shell);
char    *get_var_name(char *word, int *i);
char    *get_env_value(char *name, t_env *env);
char    *remove_quotes(char *word);

四、一步一步做
第 1 步：遍历每个 command 的 argv

你的 parser 最后大概会得到：

cmd->argv = {"echo", "$USER", NULL};

expansion 要对每一个参数做处理：

i = 0;
while (cmd->argv[i])
{
    cmd->argv[i] = expand_word(cmd->argv[i], shell);
    i++;
}

第 2 步：写 expand_word

它负责处理一个字符串。

比如：

"$USER hello"

你要一个字符一个字符看：

遇到普通字符 → 复制
遇到单引号 → 进入单引号模式
遇到双引号 → 进入双引号模式
遇到 $ → 看当前是不是允许展开

核心逻辑：

如果在单引号里：
    原样复制，不展开 $

如果不在单引号里：
    遇到 $USER 就展开

第 3 步：处理 $

几种情况：

$USER
$HOME
$?
$我会按 minishell 的真实执行顺序讲：先说明 expansion 是干嘛的，再拆成函数和步骤，最后用几个命令例子串起来。

minishell 的 expansion，白话说就是：

把用户输入里的变量、引号处理成真正要执行的字符串。

比如用户输入：

echo "$USER"

lexer/parser 阶段可能只是保存：

echo
"$USER"

到了 expansion 阶段，要变成：

echo
chen

一、expansion 在 minishell 里的位置

整体顺序大概是：

readline
   ↓
lexer 分词
   ↓
parser 组成 command
   ↓
expansion 展开变量 / 处理引号
   ↓
executor 执行

也就是说：

lexer 不展开变量，parser 也不展开变量。

真正处理 $USER、$?、引号删除，是 expansion 做。
二、expansion 主要做 3 件事
1. 变量展开

处理这种东西：

$USER
$HOME
$?

例如：

echo $HOME

变成：

echo /home/chen

$? 是上一条命令的退出状态：

echo $?

如果上一条成功，就是：

0

2. 区分单引号和双引号

规则非常重要：
单引号 '...'

里面什么都不展开。

echo '$USER'

输出：

$USER

所以 expansion 遇到单引号里面的 $，不要处理。
双引号 "..."

里面的变量要展开。

echo "$USER"

输出：

chen

所以 expansion 遇到双引号里面的 $USER，要展开。
3. 删除引号

执行前，引号本身不应该传给程序。

比如：

echo "hello world"

最终传给 execve 的参数应该是：

argv[0] = "echo";
argv[1] = "hello world";

不是：

argv[1] = "\"hello world\"";

所以 expansion 最后要把引号去掉。
三、建议你把 expansion 分成几个函数

可以这样分：

char    *expand_word(char *word, t_shell *shell);
char    *expand_var(char *word, int *i, t_shell *shell);
char    *get_var_name(char *word, int *i);
char    *get_env_value(char *name, t_env *env);
char    *remove_quotes(char *word);

四、一步一步做
第 1 步：遍历每个 command 的 argv

你的 parser 最后大概会得到：

cmd->argv = {"echo", "$USER", NULL};

expansion 要对每一个参数做处理：

i = 0;
while (cmd->argv[i])
{
    cmd->argv[i] = expand_word(cmd->argv[i], shell);
    i++;
}

第 2 步：写 expand_word

它负责处理一个字符串。

比如：

"$USER hello"

你要一个字符一个字符看：

遇到普通字符 → 复制
遇到单引号 → 进入单引号模式
遇到双引号 → 进入双引号模式
遇到 $ → 看当前是不是允许展开

核心逻辑：

如果在单引号里：
    原样复制，不展开 $

如果不在单引号里：
    遇到 $USER 就展开

第 3 步：处理 $

几种情况：

$USER
$HOME
$?
$

规则：

$?      → shell->exit_status
$USER   → 去 env 里面找 USER
$HOME   → 去 env 里面找 HOME
$不存在 → 变成空字符串
单独 $  → 保留 $

例子：

echo $ABC

如果环境变量里没有 ABC，最后相当于：

echo

第 4 步：删除引号

比如原字符串：

"hello world"

expansion 后应该是：

hello world

单引号也是：

'hello'

变成：

hello

但是注意：

'$USER'

变成：

$USER

不是 chen。
五、最重要的例子
例子 1

输入：

echo $USER

lexer/parser：

argv[0] = "echo";
argv[1] = "$USER";

expansion 后：

argv[1] = "chen";

例子 2

输入：

echo '$USER'

单引号不展开。

expansion 后：

argv[1] = "$USER";我会按 minishell 的真实执行顺序讲：先说明 expansion 是干嘛的，再拆成函数和步骤，最后用几个命令例子串起来。

minishell 的 expansion，白话说就是：

把用户输入里的变量、引号处理成真正要执行的字符串。

比如用户输入：

echo "$USER"

lexer/parser 阶段可能只是保存：

echo
"$USER"

到了 expansion 阶段，要变成：

echo
chen

一、expansion 在 minishell 里的位置

整体顺序大概是：

readline
   ↓
lexer 分词
   ↓
parser 组成 command
   ↓
expansion 展开变量 / 处理引号
   ↓
executor 执行

也就是说：

lexer 不展开变量，parser 也不展开变量。

真正处理 $USER、$?、引号删除，是 expansion 做。
二、expansion 主要做 3 件事
1. 变量展开

处理这种东西：

$USER
$HOME
$?

例如：

echo $HOME

变成：

echo /home/chen

$? 是上一条命令的退出状态：

echo $?

如果上一条成功，就是：

0

2. 区分单引号和双引号

规则非常重要：
单引号 '...'

里面什么都不展开。

echo '$USER'

输出：

$USER

所以 expansion 遇到单引号里面的 $，不要处理。
双引号 "..."

里面的变量要展开。

echo "$USER"

输出：

chen

所以 expansion 遇到双引号里面的 $USER，要展开。
3. 删除引号

执行前，引号本身不应该传给程序。

比如：

echo "hello world"

最终传给 execve 的参数应该是：

argv[0] = "echo";
argv[1] = "hello world";

不是：

argv[1] = "\"hello world\"";

所以 expansion 最后要把引号去掉。
三、建议你把 expansion 分成几个函数

可以这样分：

char    *expand_word(char *word, t_shell *shell);
char    *expand_var(char *word, int *i, t_shell *shell);
char    *get_var_name(char *word, int *i);
char    *get_env_value(char *name, t_env *env);
char    *remove_quotes(char *word);

四、一步一步做
第 1 步：遍历每个 command 的 argv

你的 parser 最后大概会得到：

cmd->argv = {"echo", "$USER", NULL};

expansion 要对每一个参数做处理：

i = 0;
while (cmd->argv[i])
{
    cmd->argv[i] = expand_word(cmd->argv[i], shell);
    i++;
}

第 2 步：写 expand_word

它负责处理一个字符串。

比如：

"$USER hello"

你要一个字符一个字符看：

遇到普通字符 → 复制
遇到单引号 → 进入单引号模式
遇到双引号 → 进入双引号模式
遇到 $ → 看当前是不是允许展开

核心逻辑：

如果在单引号里：
    原样复制，不展开 $

如果不在单引号里：
    遇到 $USER 就展开

第 3 步：处理 $

几种情况：

$USER
$HOME
$?
$

规则：

$?      → shell->exit_status
$USER   → 去 env 里面找 USER
$HOME   → 去 env 里面找 HOME
$不存在 → 变成空字符串
单独 $  → 保留 $

例子：

echo $ABC

如果环境变量里没有 ABC，最后相当于：

echo

第 4 步：删除引号

比如原字符串：

"hello world"

expansion 后应该是：

hello world

单引号也是：

'hello'

变成：

hello

但是注意：

'$USER'

变成：

$USER

不是 chen。
五、最重要的例子
例子 1

输入：

echo $USER

lexer/parser：

argv[0] = "echo";
argv[1] = "$USER";

expansion 后：

argv[1] = "chen";

例子 2

输入：

echo '$USER'

单引号不展开。

expansion 后：

argv[1] = "$USER";

例子 3

输入：

echo "$USER"

双引号展开。

expansion 后：

argv[1] = "chen";

例子 4

输入：

echo hello"$USER"test

假设 USER=chen

最后应该变成：

hellochentest

这是一个参数，不是三个参数。
六、你可以先实现的最小版本

先支持这些就够了：

$VAR
$?
单引号不展开
双引号展开
最后删除引号

暂时不要一开始就处理太复杂的：

word splitting
通配符 *
转义符 \

minishell 题目一般不要求你完整实现 bash 的所有 expansion。
七、最推荐的 expansion 流程

对每个 cmd
    对每个 argv[i]
        1. expand_word(argv[i])
            - 处理 $VAR
            - 处理 $?
            - 区分单双引号
        2. remove_quotes(argv[i])
            - 删除 ' 和 "

简单记：

先展开变量
再删除引号

但是展开变量时必须知道自己是不是在单引号里面。

一句话总结：

expansion 就是把 $变量 换成真实值，同时根据单双引号规则决定要不要换，最后把引号删掉。

例子 3

输入：

echo "$USER"

双引号展开。

expansion 后：

argv[1] = "chen";

例子 4

输入：

echo hello"$USER"test

假设 USER=chen

最后应该变成：

hellochentest

这是一个参数，不是三个参数。
六、你可以先实现的最小版本

先支持这些就够了：

$VAR
$?
单引号不展开
双引号展开
最后删除引号

暂时不要一开始就处理太复杂的：

word splitting
通配符 *
转义符 \

minishell 题目一般不要求你完整实现 bash 的所有 expansion。
七、最推荐的 expansion 流程

对每个 cmd
    对每个 argv[i]
        1. expand_word(argv[i])
            - 处理 $VAR
            - 处理 $?
            - 区分单双引号
        2. remove_quotes(argv[i])
            - 删除 ' 和 "

简单记：

先展开变量
再删除引号

但是展开变量时必须知道自己是不是在单引号里面。

一句话总结：

expansion 就是把 $变量 换成真实值，同时根据单双引号规则决定要不要换，最后把引号删掉。

规则：

$?      → shell->exit_status
$USER   → 去 env 里面找 USER
$HOME   → 去 env 里面找 HOME
$不存在 → 变成空字符串
单独 $  → 保留 $

例子：

echo $ABC

如果环境变量里没有 ABC，最后相当于：

echo

第 4 步：删除引号

比如原字符串：

"hello world"

expansion 后应该是：

hello world

单引号也是：

'hello'

变成：

hello

但是注意：

'$USER'

变成：

$USER

不是 chen。
五、最重要的例子
例子 1

输入：

echo $USER

lexer/parser：

argv[0] = "echo";
argv[1] = "$USER";

expansion 后：

argv[1] = "chen";

例子 2

输入：

echo '$USER'

单引号不展开。

expansion 后：

argv[1] = "$USER";

例子 3

输入：

echo "$USER"

双引号展开。

expansion 后：

argv[1] = "chen";

例子 4

输入：

echo hello"$USER"test

假设 USER=chen

最后应该变成：

hellochentest

这是一个参数，不是三个参数。
六、你可以先实现的最小版本

先支持这些就够了：

$VAR
$?
单引号不展开
双引号展开
最后删除引号

暂时不要一开始就处理太复杂的：

word splitting
通配符 *
转义符 \

minishell 题目一般不要求你完整实现 bash 的所有 expansion。
七、最推荐的 expansion 流程

对每个 cmd
    对每个 argv[i]
        1. expand_word(argv[i])
            - 处理 $VAR
            - 处理 $?
            - 区分单双引号
        2. remove_quotes(argv[i])
            - 删除 ' 和 "

简单记：

先展开变量
再删除引号

但是展开变量时必须知道自己是不是在单引号里面。

一句话总结：

expansion 就是把 $变量 换成真实值，同时根据单双引号规则决定要不要换，最后把引号删掉。

例子 3

输入：

echo "$USER"

双引号展开。

expansion 后：

argv[1] = "chen";

例子 4

输入：

echo hello"$USER"test

假设 USER=chen

最后应该变成：

hellochentest

这是一个参数，不是三个参数。
六、你可以先实现的最小版本

先支持这些就够了：

$VAR
$?
单引号不展开
双引号展开
最后删除引号

暂时不要一开始就处理太复杂的：

word splitting
通配符 *
转义符 \

minishell 题目一般不要求你完整实现 bash 的所有 expansion。
七、最推荐的 expansion 流程

对每个 cmd
    对每个 argv[i]
        1. expand_word(argv[i])
            - 处理 $VAR
            - 处理 $?
            - 区分单双引号
        2. remove_quotes(argv[i])
            - 删除 ' 和 "

简单记：

先展开变量
再删除引号

但是展开变量时必须知道自己是不是在单引号里面。

一句话总结：

expansion 就是把 $变量 换成真实值，同时根据单双引号规则决定要不要换，最后把引号删掉。
argv[1] = "$USER";

expansion 后：

argv[1] = "chen";
例子 2

输入：

echo '$USER'

单引号不展开。

expansion 后：

argv[1] = "$USER";
例子 3

输入：

echo "$USER"

双引号展开。

expansion 后：

argv[1] = "chen";
例子 4

输入：

echo hello"$USER"test

假设 USER=chen

最后应该变成：

hellochentest

这是一个参数，不是三个参数。

六、你可以先实现的最小版本

先支持这些就够了：

$VAR
$?
单引号不展开
双引号展开
最后删除引号

暂时不要一开始就处理太复杂的：

word splitting
通配符 *
转义符 \

minishell 题目一般不要求你完整实现 bash 的所有 expansion。

七、最推荐的 expansion 流程
对每个 cmd
    对每个 argv[i]
        1. expand_word(argv[i])
            - 处理 $VAR
            - 处理 $?
            - 区分单双引号
        2. remove_quotes(argv[i])
            - 删除 ' 和 "

简单记：

先展开变量
再删除引号

但是展开变量时必须知道自己是不是在单引号里面。

一句话总结：

expansion 就是把 $变量 换成真实值，同时根据单双引号规则决定要不要换，最后把引号删掉。
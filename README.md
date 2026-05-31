
*This project has been created as part of the 42 curriculum by yushli, xingchen.*


阶段1 
Lexer 和 parser
阶段2
Expansion + env +heredoc和 executor +pipe + redir 
阶段3
Builtin 和 signal
阶段4 
Debug
-----------------------------------------------------------------------------------
Claude 建议
你的计划
阶段	人A	人B
1	Lexer	Parser
2	Expansion + env	Executor + pipe + redir + heredoc
3	Builtin	Signal
4	一起 Debug	
________________________________________
问题点
阶段2 工作量严重不平衡
Executor + pipe + redir + heredoc 是整个项目最难最大的部分，而 Expansion + env 相对小很多。
建议调整：
阶段2	人A	人B
调整后	Expansion + env + heredoc	Executor + pipe + redir
heredoc 本质是处理输入内容，跟 expansion 更接近，放一起更合理。
________________________________________
阶段3 也不平衡
Builtin 有 7 个（echo、cd、pwd、export、unset、env、exit），工作量比 signal 大很多。
建议：
阶段3	人A	人B
调整后	Builtin（cd/pwd/export/unset）	Builtin（echo/env/exit）+ Signal
---------------------------------------------------------------------------------------------------------
重定向
改变命令的输入来源或输出目的地。
------------------------------------------------------------------------------------------------------------
Lexer
把用户输入的字符串，切成 token 链表。

输入 → 输出
输入: echo "hello world" | cat > out.txt

输出:
[WORD:"echo"] → [WORD:"hello world"] → [PIPE] → [WORD:"cat"] → [REDIR_OUT] → [WORD:"out.txt"] → NULL

具体要做的事
1. 跳过空格
"  echo  hello"  →  不是token，跳过
2. 识别操作符
|   →  TOKEN_PIPE
<   →  TOKEN_REDIR_IN
>   →  TOKEN_REDIR_OUT
>>  →  TOKEN_REDIR_APPEND
<<  →  TOKEN_HEREDOC
3. 识别普通单词
echo、hello、-v、foo  →  TOKEN_WORD
4. 处理引号
"hello world"  →  TOKEN_WORD, value = "hello world"  (引号去掉，空格保留)
'$USER'        →  TOKEN_WORD, value = "$USER"         (引号去掉，内容原样)

--------------------------------------------------------------------------------------------

Parser
把 token 链表，转成 cmd 链表。

输入 → 输出
输入 (来自Lexer):
[WORD:"echo"] → [WORD:"hello"] → [PIPE] → [WORD:"cat"] → [REDIR_OUT] → [WORD:"out.txt"] → NULL

输出:
t_cmd1:                    t_cmd2:
  argv  = ["echo","hello"]   argv  = ["cat"]
  redirs = NULL              redirs = [REDIR_OUT, "out.txt"]
  next  → t_cmd2             next  = NULL

具体要做的事
1. 遇到 WORD → 放进当前 cmd 的 argv
[WORD:"grep"] [WORD:"-v"] [WORD:"foo"]
→ argv = ["grep", "-v", "foo", NULL]
2. 遇到重定向 → 把下一个 WORD 作为文件名，放进 redirs
[REDIR_OUT] [WORD:"out.txt"]
→ redirs = {type: REDIR_OUT, file: "out.txt"}
3. 遇到 PIPE → 新建下一个 cmd
[PIPE]
→ 当前cmd结束，next指向新cmd
4. 语法错误检查
| | |        →  报错，pipe后面不能是pipe
echo >       →  报错，重定向后面没有文件名


输出结构体
ctypedef struct s_redir {
    
} t_redir;

typedef struct s_cmd {
    
} t_cmd;

--------------------------------------------------------------------------------------
Expansion
把 argv 里的 $VAR 替换成真实的值。

输入 → 输出
Parser 输出的 argv:
["echo", "$USER", "$?", NULL]

Expansion 处理后:
["echo", "alice", "0", NULL]
具体要做的事
1. $VAR 展开
bashecho $HOME
→ echo /home/alice
2. $? 展开
bashecho $?
→ echo 0   (上一条命令的退出码)
3. 引号内的区别
bashecho "$USER"   →  展开，输出 alice
echo '$USER'   →  不展开，输出 $USER

Env 
管理环境变量的存储、查找、增删。
具体要做的事
1. 存储环境变量
c// 把 envp 转成自己的链表或数组，方便操作
["USER=alice", "HOME=/home/alice", NULL]
2. 查找变量
bash$HOME  →  去env里找 "HOME"，返回 "/home/alice"
3. 增删变量（给builtin用）
bashexport FOO=bar   →  添加进env
unset FOO        →  从env删除

两者关系
env 是仓库        →   存变量
expansion 是使用者 →   去仓库取变量来替换

在整个流程里的位置
Lexer → Parser → Expansion → Executor
                 ↑
            在执行前把$VAR替换好
            executor拿到的已经是真实值

----------------------------------------------------------------------
Executor 
拿到 cmd 链表，真正去执行每条命令。

在流程里的位置
Lexer → Parser → Expansion → Executor
                              ↑
                         这里才真正运行命令

具体要做的事
1. 找到命令的路径
bashgrep -v foo
→ 去 $PATH 里找 grep 在哪
→ /usr/bin/grep
2. fork + execve 执行命令
cpid = fork();
if (pid == 0)
    execve("/usr/bin/grep", argv, envp); // 子进程执行
else
    waitpid(pid, &status, 0); // 父进程等待
3. 处理重定向（redir）
bashcat < in.txt > out.txt
→ 用 dup2 把stdin换成in.txt
→ 用 dup2 把stdout换成out.txt
→ 再执行 cat
4. 处理管道（pipe）
bashecho hello | grep h | cat
→ 创建pipe
→ 每条命令fork一个子进程
→ 上一条的stdout接下一条的stdin
5. 处理内置命令
bashcd /home
→ 不能fork，必须在父进程直接执行
→ 判断是builtin就直接调用，不execve

执行流程
拿到 t_cmd 链表
      ↓
是否是 builtin？
  ↓是              ↓否
直接执行         fork子进程
                    ↓
               设置重定向(dup2)
                    ↓
               execve执行
                    ↓
               父进程waitpid

管道的情况
bashcmd1 | cmd2 | cmd3

→ 创建2个pipe
→ fork 3个子进程
→ cmd1的stdout → pipe1 → cmd2的stdin
→ cmd2的stdout → pipe2 → cmd3的stdin
→ 父进程等所有子进程结束

--------------------------------------------------------------------
Builtin 要干什么
实现 shell 自带的命令。

为什么需要 builtin？
有些命令不能用 execve 执行，必须在当前进程里运行：
bashcd /home
# 如果fork子进程去执行，子进程改了目录
# 父进程(shell)的目录根本不会变
# 所以必须在父进程直接执行

7个必须实现的 builtin
1. echo
bashecho hello        → 打印 hello 加换行
echo -n hello     →  打印 hello 不换行
2. cd
bashcd /home/alice    → 改变当前目录
cd                → 回到 $HOME
3. pwd
bashpwd               → 打印当前目录路径
4. export
bashexport FOO=bar    → 添加/修改环境变量
export            → 打印所有环境变量
5. unset
bashunset FOO         → 删除环境变量
6. env
bashenv               → 打印所有环境变量
7. exit
bashexit              → 退出shell
exit 42           → 退出并返回码42
-----------------------------------------------------------------------------
Signal
处理键盘中断信号，让 shell 行为正常。

3个必须处理的信号
1. ctrl+C (SIGINT)
bash# 执行中:
sleep 100
^C
→ 终止当前命令，回到新的命令行提示符

# 空命令行:
^C
→ 不退出shell，只是换一行
2. ctrl+D (EOF)
bash# 空命令行输入ctrl+D
→ 退出shell（等同于exit）
3. ctrl+\ (SIGQUIT)
bash# 执行中忽略或终止
# 空命令行完全忽略

两种状态下信号行为不同
状态ctrl+Cctrl+Dctrl+\等待输入时新提示符退出shell忽略执行命令时终止命令忽略忽略


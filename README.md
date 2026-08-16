
*This project has been created as part of the 42 curriculum by yushli, xingchen.*

### Check the command type

To check whether a command is a Bash builtin or an external executable, use:

```bash
type <command>
```

Examples:

```bash
type cd
# cd is a shell builtin
type :
# : is a shell builtin
type ls
# ls is /usr/bin/ls
```
Note: `type` is itself a Bash builtin, so this check must be performed in Bash, not in Minishell.  
If there is any doubt about whether a command is a Bash builtin or an external executable, exit Minishell and use `type <command>` in Bash to verify it.
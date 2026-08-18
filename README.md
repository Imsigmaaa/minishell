
*This project has been created as part of the 42 curriculum by yushli, xingchen.*

# Minishell

Minishell is a simple shell program written in C. The goal of this project is to reproduce some Bash behaviors and understand how a shell works.
It handles commands, pipes, redirections, signals, environment variables, built-in commands, parsing, and command execution.

## Description

The goal of this project is to create a simple shell and understand how Bash works. Minishell reads the user input, analyzes the command, expands variables, prepares redirections and heredocs, and then executes built-in or external commands.

The command flow works like this:

1. **Input** - Read the command line with GNU Readline and add it to the history.
2. **Lexing** - Split the input into tokens like words, pipes, redirections, such as `<`, `>`, `>>`, and `<<`, while keeping quote information.
3. **Syntax checking** - Check for errors like unclosed quotes, invalid pipes, redirections, or unsupported operators.
4. **Parsing** - Create the command structure with arguments and redirections.
5. **Expansion** - Expand environment variables and `$?`, depending on single or double quotes.
6. **Heredoc** - Read the heredoc before execution and expand variables if the delimiter is not quoted.
7. **Execution** - Execute built-in or external commands, find commands using `PATH`, create pipes, handle redirections, and get the exit status.

### Implemented features

- Interactive prompt using GNU Readline
- Command history
- Execute external commands using `PATH`, relative paths, or absolute paths
- Empty parts in `PATH` work as the current directory
- Single and double quotes
- Environment variables
- Environment variable expansion with `$VAR`
- Exit status expansion with `$?`
- Input redirection: `<`
- Output redirection: `>`
- Append redirection: `>>`
- Heredoc: `<<`
- Multiple pipes: `|`
- Handle `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`
- Exit status handling for commands, errors, and signals

### Built-in commands

| Command | Behavior |
|---|---|
| `echo` | Prints text, with support for the `-n` option |
| `cd` | Changes the current directory using a relative or absolute path |
| `pwd` | Prints the current working directory |
| `export` | Adds, updates, or displays environment variables |
| `unset` | Removes environment variables |
| `env` | Prints environment variables that have values |
| `exit` | Exits Minishell with an optional numeric exit status |

According to the subject, only `echo` needs to support an option (`-n`). `pwd`, `export`, `unset`, and `exit` do not need to support options. `env` does not need to support options or arguments.

### Project structure
```text
.
├── includes/           Header files
├── libft/              Utility functions
├── srcs/
│   ├── main.c          Main loop
│   ├── init.c          Initialization
│   ├── builtin/        Built-in commands
│   ├── env/            Environment
│   ├── exec/           Execution, pipes, and redirections
│   ├── expand/         Expansion
│   ├── heredoc/        Heredoc
│   ├── lexer/          Lexer
│   ├── parser/         Parser
│   └── signal/         Signals
├── Makefile
└── README.md           Project documentation
```

## Instructions

### Requirements

You need `cc`, `make`, and GNU Readline.

Install Readline if needed:

**Debian / Ubuntu**
```bash
sudo apt install libreadline-dev
```

**Fedora**
```bash
sudo dnf install readline-devel
```

**macOS**
```bash
brew install readline
```

### Compilation

Compile the project with:

```bash
make
```

Available commands:

```bash
make          # Build minishell
make clean    # Remove object files
make fclean   # Remove object files and minishell
make re       # Rebuild the project
```

### Execution

Run Minishell with:

```bash
./minishell
```

Example:

```console
minishell$ echo "Hello $USER"
Hello user

minishell$ pwd
/home/user/minishell

minishell$ echo hello | grep h > result.txt

minishell$ cat << EOF | wc -l
> first line
> second line
> EOF
2

minishell$ echo $?
0

minishell$ exit
```
### Leak check

To check memory leaks and open file descriptors:

```bash
valgrind --leak-check=full --track-fds=yes ./minishell
```
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

## Technical choices

- The environment is stored in a linked list and converted to `char **` for `execve`.
- Commands and redirections are stored in linked lists.
- A single built-in runs in the parent process, while built-ins in pipes run in child processes.
- We use `dup` to save standard input and output, and `dup2` to handle pipes and redirections.
- Each command in a pipe runs in its own child process and communicates through `pipe`.
- Heredocs are prepared before execution. If a heredoc is interrupted, the commands are not executed.
- We use only one global `sig_atomic_t` variable to store the signal number.

## Limitations

This project implements the mandatory part of the 42 Minishell subject.

Bonus features such as `&&`, `||`, parentheses `()`, and wildcards `*` are not implemented.

For other shell behaviors, Bash is used as a reference.

## Resources

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [Linux manual pages](https://man7.org/linux/man-pages/)
Some functions and system calls used:

- `fork`, `execve`, `pipe`
- `dup`, `dup2`
- `waitpid`
- `signal`, `sigaction`
- `open`, `close`, `unlink`
- etc.

### Use of AI

AI tools were used as a learning aid during this project.

They were used for:

- Explaining Unix concepts like processes, pipes, file descriptors, signals, and exit status
- Helping us find useful information about Unix and shell behavior

The implementation, testing, and final decisions were done by us.

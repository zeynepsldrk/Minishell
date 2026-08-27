*This project has been created as part of the 42 curriculum by asay, zedurak*


# Minishell


## Description

minishell is a shell program written in C, built as part of the 42 curriculum. The goal is to reproduce the core behavior of bash — reading a command from the user, parsing it, and executing it correctly.

The project covers the full pipeline from input to output: tokenizing raw text, handling quotes and variable expansion, building a command structure, and finally executing processes with pipes and redirections.

**Supported features:**

- Interactive prompt with command history
- Single quotes `'...'` — treats content literally, no expansion
- Double quotes `"..."` — allows variable expansion inside
- Environment variable expansion: `$VAR` and `$?`
- Redirections: `<` `>` `>>` and `<<` (heredoc)
- Pipes `|` to chain commands
- Signal handling: `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
- Syntax error detection for malformed input
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

## Instructions

**Dependencies**

The project requires the `readline` library, which is available on 42 school machines.

**Compilation**

```bash
git clone https://github.com/lenas3/Minishell.git
cd Minishell
make
```

**Running**

```bash
./minishell
```

Once launched, you can type commands just like in bash:

```
minishell$ echo "Hello, $USER!"
Hello, aleyna!
minishell$ ls | grep ".c" > files.txt
minishell$ export VAR=42 && echo $VAR
42
minishell$ cat << END
> some heredoc input
> END
minishell$ exit
```

**Makefile targets**

```bash
make        # compile
make clean  # remove object files
make fclean # remove object files and binary
make re     # recompile from scratch
```

## Resources

**Documentation**

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — main reference for bash behavior
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — formal shell grammar and semantics
- [readline(3) man page](https://man7.org/linux/man-pages/man3/readline.3.html) — interactive input library

- [Youtube Shell Program Explainede](https://www.youtube.com/watch?v=ubt-UjcQUYg&list=PL2POs8ZJ4I2JJK3lICRW0b-L2y9mLiDmU&index=2) - overview of how a shell program works

- [Youtube Shell Code Details](https://www.youtube.com/watch?v=ZjzMdsTWF0U&list=PL2POs8ZJ4I2JJK3lICRW0b-L2y9mLiDmU&index=3) - deeper look into shell implementation details

**Relevant man pages**

`fork(2)`, `execve(2)`, `waitpid(2)`, `pipe(2)`, `dup2(2)`, `signal(2)`, `sigaction(2)`

**Articles**

- [Writing a Simple Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/) — Stephen Brennan


**AI Usage**

Claude and Gemini were used in the following areas:


Fixing failing test cases by analyzing incorrect output and tracing the source of bugs
Writing small helper functions (e.g. string utilities, simple getters)
Reformatting code to comply with the 42 Norm
Understanding concepts and clarifying how certain shell behaviors work


AI was not used to write the core logic of the project. All design decisions and implementations were made and understood by the authors.
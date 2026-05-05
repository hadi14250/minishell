
<div align="center">
<a href="https://github.com/hadi14250">
    <img src="github_gifs/ansi.gif" alt="3d cube" width="150" height="150">
  </a>
  <h3 align="center">Minishell</h3>
  A small Unix shell, written from scratch in C
  <br>
  <br>
</div>


<div align="center">
<a href="https://github.com/hadi14250">
    <img src="github_gifs/minishell.gif" alt="gif gameplay"  width="600" height="350">
  </a>


</div>

<br>


# Minishell

A minimal recreation of a Unix shell, built in pure C. The project follows the full journey of a typed line through a real shell — reading input, breaking it into tokens, parsing those tokens into a command tree, and executing them with the right plumbing for pipes, redirections, and signals. It is a hands-on exercise in process management, file descriptors, and how the terminal actually talks to your programs.

<br>

## Getting started

```sh
make           # build the binary
./minishell    # launch the shell
```

A prompt appears — start typing commands as you would in bash.

<br>

## Features

### Prompt and history
- Interactive prompt with line editing
- Command history navigated with the up and down arrow keys

### Command execution
- Runs any executable found through the `PATH` environment variable (`ls`, `cat`, `grep`, and so on)

### Builtins
| Command  | Description                                       |
| -------- | ------------------------------------------------- |
| `echo`   | prints arguments, supports the `-n` flag          |
| `cd`     | changes directory using a relative or absolute path |
| `pwd`    | prints the current working directory              |
| `export` | adds or updates environment variables             |
| `unset`  | removes environment variables                     |
| `env`    | prints the current environment                    |
| `exit`   | exits the shell with an optional exit code        |

### Pipes
- `|` connects the output of one command to the input of the next

### Redirections
- `>`  — writes output to a file, replacing it
- `>>` — writes output to a file, appending to it
- `<`  — reads input from a file
- `<< DELIMITER` — opens a here-document; input is read line by line until `DELIMITER` is entered, then fed to the command

### Variable expansion
- `$VAR` expands to the value of an environment variable
- `$?` expands to the exit status of the most recent foreground pipeline

### Signal handling
- `ctrl-c` cancels the current input and shows a fresh prompt (SIGINT)
- `ctrl-d` on an empty line exits the shell
- `ctrl-\` is ignored, matching bash behaviour

<br>

## How it works

- **Lexer** — turns the raw input string into a stream of tokens, handling quoting and variable expansion along the way
- **Parser** — validates the token stream, reports syntax errors, and assembles the structured command tree the executor runs
- **Executor** — forks processes, wires up pipes and redirections, dispatches builtins, and collects exit statuses

<br>

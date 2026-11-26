_This project has been created as part of the 42 curriculum by pabartoc, esttina._

# 🐚 minishell - 42 Project

## 📖 Description

**minishell** is a systems programming project in the 42 School curriculum whose goal is to build a minimal, POSIX-inspired UNIX command-line interpreter (shell) in C, modeled after **GNU Bash**.

The primary objective of this project is to gain a deep understanding of **process creation and synchronization** (`fork`, `execve`, `waitpid`), **inter-process communication (IPC)** via UNIX pipes (`pipe`), **file descriptor manipulation** (`open`, `close`, `dup2`), **signal handling** (`sigaction`), and **lexical analysis / parsing** (tokenization, state-machine quote handling, syntax validation, and environment variable expansion).

---

## 🔄 How It Works — Visual Pipeline Walkthrough

To understand what happens inside **minishell** when a user types a command and presses `Enter`, let's trace a single input line from raw keystrokes all the way to operating system processes:

```bash
minishell$ echo "Hi $USER" | wc -c > out.txt
```

### 1. Raw Input (`readline`)
The shell reads the user's keystrokes as one flat C string in memory and records it in the command history (`add_history`):
```text
"echo \"Hi $USER\" | wc -c > out.txt"
```

### 2. Lexical Analysis / Tokenization (`t_token`)
The **Lexer** scans the string character by character using a quote state-machine (`0`, `'`, `"`). It splits words at spaces and isolates operators (`|`, `<`, `>`, `<<`, `>>`), while keeping quoted blocks together as a single token:
```text
[WORD: echo] ──► [WORD: "Hi $USER"] ──► [PIPE: |] ──► [WORD: wc] ──► [WORD: -c] ──► [REDIR_OUT: >] ──► [WORD: out.txt]
```

### 3. Syntax Validation & Variable Expansion (`expander.c`)
First, the **Syntax Validator** checks the grammar rules (no leading/consecutive pipes, every redirection is followed by a filename word).
Next, the **Expander** looks up `$USER` in the shell environment (because it is inside double quotes `"..."`, not locked inside single quotes `'...'`), replaces it with `"pabartoc"`, and strips the outer quote characters:
```text
[WORD: echo] ──► [WORD: Hi pabartoc] ──► [PIPE: |] ──► [WORD: wc] ──► [WORD: -c] ──► [REDIR_OUT: >] ──► [WORD: out.txt]
```

### 4. Command Table Construction (`t_cmd` — The Parser/Executor Contract)
The **Command Builder** groups all tokens between pipes (`|`) into self-contained command nodes (`t_cmd`). Normal words are packed into a `NULL`-terminated `args` array (ready for `execve`), while redirections and their target filenames are extracted into a separate `redirs` list:

```text
┌─────────────────────────────────┐        ┌─────────────────────────────────┐
│              CMD 0              │        │              CMD 1              │
├─────────────────────────────────┤        ├─────────────────────────────────┤
│ args[0]: "echo"                 │        │ args[0]: "wc"                   │
│ args[1]: "Hi pabartoc"          │  ───►  │ args[1]: "-c"                   │
│ args[2]: NULL                   │  next  │ args[2]: NULL                   │
│                                 │        │                                 │
│ redirs:  NULL                   │        │ redirs:  [TYPE: > , "out.txt"]  │
└─────────────────────────────────┘        └─────────────────────────────────┘
```

### 5. Execution Dispatch: Parent Built-in vs. Child Processes (`exec_cmds`)
Once the Parser hands the `t_cmd` list over to the **Executor**, `exec_cmds` decides *where* the command must run:
* **Single Built-in Command (`cmds->next == NULL` and `is_builtin`):** Built-ins such as `cd`, `export`, `unset`, and `exit` must modify the state of the main shell itself. Therefore, they execute **directly inside the Parent Process** without `fork()`. To support redirections on parent built-ins (e.g., `echo hi > out.txt`), `exec_builtin_redir` temporarily backs up `STDIN (0)` and `STDOUT (1)` using `dup()`, applies the redirections, runs the built-in, and restores the original terminal descriptors via `dup2()`.
* **External Programs or Pipelines (`cmds->next != NULL`):** The shell spawns isolated **Child Processes** via `fork()`.

```text
                         ┌──────────────────────┐
                         │  exec_cmds(t_cmd *)  │
                         └──────────┬───────────┘
                                    │
                  ┌─────────────────┴─────────────────┐
                  ▼ (Single Built-in)                 ▼ (External Cmd or Pipeline)
    ┌───────────────────────────┐       ┌───────────────────────────┐
    │      Parent Process       │       │   fork() Child Process(es)│
    │ 1. dup(0), dup(1) backup  │       │ 1. Wire pipe() descriptors│
    │ 2. apply_redirs()         │       │ 2. apply_redirs()         │
    │ 3. exec_builtin()         │       │ 3. find_path() in $PATH   │
    │ 4. dup2() restore & close │       │ 4. execve() / exit()      │
    └───────────────────────────┘       └───────────────────────────┘
```

### 6. Pipeline Wiring & File Descriptor Management (`exec_pipeline`)
For a pipeline (`CMD 0 | CMD 1`), the Executor iterates through the `t_cmd` list, creating a kernel pipe (`pipe(pfd)`) before each `fork()` and passing the read-end (`prev_fd = pfd[0]`) to the next command:

```text
┌────────────────────────┐      pipe(pfd)    ┌────────────────────────┐     open("out.txt")   ┌───────────┐
│  Child 1 (CMD 0: echo) │ ════════════════► │  Child 2 (CMD 1: wc)   │ ════════════════════► │  out.txt  │
│  stdout ──► pfd[1]     │   "Hi pabartoc\n" │  stdin  ◄── prev_fd    │        "12\n"         │  (on disk)│
│  (closes pfd[0])       │                   │  stdout ──► file_fd    │                       └───────────┘
└────────────────────────┘                   └────────────────────────┘
```
* **Crucial Rule:** Every unused pipe end (`pfd[0]`, `pfd[1]`, `prev_fd`) is immediately closed (`close()`) in both the parent and child processes so that downstream readers receive an `EOF` signal as soon as upstream writers finish.

### 7. Redirections & Heredoc (`apply_redirs`)
Inside each command, `apply_redirs` walks through the `cmd->redirs` list from left to right. Any file redirection opened after a pipe overrides the pipe descriptor via `dup2(fd, target_fd)`:

| Operator | System Call / Behavior | Rewired Descriptor |
| ----- | ----- | ----- |
| `< infile` | `open(target, O_RDONLY)` | `dup2(fd, STDIN_FILENO)` (`0`) |
| `> outfile` | `open(target, O_WRONLY \| O_CREAT \| O_TRUNC, 0644)` | `dup2(fd, STDOUT_FILENO)` (`1`) |
| `>> outfile` | `open(target, O_WRONLY \| O_CREAT \| O_APPEND, 0644)` | `dup2(fd, STDOUT_FILENO)` (`1`) |
| `<< LIMITER` | `handle_heredoc`: Reads lines via `readline("> ")` into an internal `pipe()` until `LIMITER` is matched | `dup2(pfd[0], STDIN_FILENO)` (`0`) |

### 8. `$PATH` Resolution, `execve` & Exit Status Synchronization (`$?`)
1. **Binary Lookup (`find_path`):** If `cmd->args[0]` contains a `'/'` (e.g., `/bin/ls` or `./minishell`), it is used directly. Otherwise, `find_path` splits `PATH` from `shell->env` at `':'`, joins each directory with `"/<cmd>"`, and checks execute permissions via `access(full_path, X_OK)`.
2. **Process Replacement (`execve`):** `execve(path, cmd->args, shell->env)` replaces the child process with the target program. If lookup or execution fails, the child exits with standard Bash status codes (`127` for command not found, `126` for permission denied / is a directory).
3. **Synchronization & Signals (`wait_pipeline` & `decode_status`):** While waiting for child processes (`setup_signals_wait`), the parent temporarily ignores `SIGINT`/`SIGQUIT` so only the running children react to `Ctrl+C` or `Ctrl+\`. Once `waitpid()` collects the last command's status, `decode_status` extracts either `WEXITSTATUS(status)` or `128 + WTERMSIG(status)` (`130` for `SIGINT`, `131` for `SIGQUIT`) and stores it in `shell.last_status` (ready for the next `$?` expansion).

---

## ✨ Features & Architecture

The shell operates as an interactive Read-Eval-Print Loop (REPL) divided into two modular subsystems connected via a clean command-table interface (`t_cmd` / `t_redir`):

| Subsystem | Module / Phase | Description |
| ----- | ----- | ----- |
| **Parsing (`pabartoc`)** | **1. Lexer (`parser.c`, `tokens_list.c`)** | Splits raw `readline` input into a linked list of typed tokens (`t_token`), recognizing words, quotes (`'`, `"`), and operators (`\|`, `<`, `>`, `<<`, `>>`) even without surrounding whitespace. |
| **Parsing (`pabartoc`)** | **2. Syntax Validator (`syntax_check.c`)** | Verifies token grammar before execution (detects unclosed quotes, leading/trailing/consecutive pipes, and missing redirection targets; sets exit status `$? = 2`). |
| **Parsing (`pabartoc`)** | **3. Expander (`expander.c`)** | Resolves `$VAR` against the shell environment and `$?` against the last foreground exit status outside single quotes, then strips enclosing quote characters. |
| **Parsing (`pabartoc`)** | **4. Command Builder (`cmd_builder.c`)** | Converts the flat token list into a pipeline-ready linked list of `t_cmd` nodes, separating `NULL`-terminated `char **args` arrays from `t_redir` redirection lists. |
| **Execution (`esttina`)** | **1. Environment (`env.c`, `env_set.c`)** | Duplicates `envp` onto the heap (`dup_env`) and provides dynamic lookup, insertion, update, and removal utilities (`get_env_value`, `set_env_value`, `unset_env_value`). |
| **Execution (`esttina`)** | **2. Builtins (`builtins.c`, `builtin_*.c`)** | Native implementations of `echo` (with `-n`), `cd`, `pwd`, `export` (including sorted output), `unset`, `env`, and `exit`. |
| **Execution (`esttina`)** | **3. Redirections & Pipes (`redirs.c`, `pipeline.c`)** | File descriptor redirection (`<`, `>`, `>>`), interactive heredoc pipes (`<<`), and multi-stage process pipelines (`pipe`, `dup2`, `fork`, `waitpid`). |
| **Execution (`esttina`)** | **4. Executor & Signals (`executor.c`, `path.c`, `signals.c`)** | `PATH` binary resolution (`access`), process exit-code decoding (`126`, `127`, `128 + sig`), and context-aware signal handling (`SIGINT`, `SIGQUIT`, `EOF`). |

---

## 📂 Project Structure

The project is organized into headers (`inc/`), source modules (`src/parsing/` and `src/exec/`), and the integrated `libft` library:

```text
minishell/
├── Makefile
├── README.md
├── inc/
│   ├── minishell.h                  # System includes & global header aggregation
│   ├── parsing.h                    # Token, redirection, and command structs & parser prototypes
│   └── exec.h                       # Shell state struct (t_shell), global signal & execution prototypes
├── libft/                           # Custom C library (including ft_printf & get_next_line)
└── src/
    ├── minishell.c                  # Main REPL loop (readline, add_history, pipeline dispatch)
    ├── parsing/
    │   ├── parser.c                 # Tokenizer / Lexer with quote state-machine
    │   ├── tokens_list.c            # Linked-list allocation & memory cleanup (free_tokens, free_cmds)
    │   ├── syntax_check.c           # Grammar validation for pipes & redirections
    │   ├── expander.c               # Environment variable ($VAR, $?) expansion & quote removal
    │   └── cmd_builder.c            # Command table (t_cmd & t_redir) construction
    └── exec/
        ├── executor.c               # Command dispatch, child execution & exit status decoding
        ├── pipeline.c               # Multi-process pipe creation, fork loop & waitpid synchronization
        ├── redirs.c                 # Input/output file redirections (<, >, >>) & heredoc (<<)
        ├── path.c                   # $PATH resolution & executable permission checks
        ├── signals.c                # Signal handlers (SIGINT, SIGQUIT) for prompt, wait & child modes
        ├── builtins.c               # Built-in command dispatcher, pwd, env & unset
        ├── builtin_cd_export.c      # Built-in cd (PWD/OLDPWD updates) & export variable assignment
        ├── builtin_echo_exit.c      # Built-in echo (-n flag) & exit (numeric validation & status)
        ├── export_print.c           # Alphabetically sorted environment output for standalone export
        ├── env.c                    # Heap environment duplication & variable lookup
        └── env_set.c                # Dynamic environment variable insertion, modification & removal
```

---

## 🛠️ Instructions

### Prerequisites

* A C compiler (`cc`, `clang`, or `gcc`)
* `make`
* GNU `readline` library (`-lreadline`)

### Compilation

Clone the repository and run `make` in the root directory:

```bash
git clone <https://github.com/pyoskel/minishell.git> minishell
cd minishell
make
```

### Makefile Rules

* `make` or `make all`: Compiles `libft` and all source files into the `minishell` executable using `-Wall -Wextra -Werror`.
* `make clean`: Removes all object files (`.o`) from `src/` and `libft/`.
* `make fclean`: Runs `clean` and removes `minishell` as well as `libft/libft.a`.
* `make re`: Performs a full rebuild (`fclean` followed by `all`).

---

## 🚀 Usage & Local Testing

Start the interactive shell by running the executable without arguments:

```bash
./minishell
```

### 1. Error Handling Tests (Edge-Cases)

```bash
# Unclosed quotes (must print syntax error without crashing or leaking)
minishell$ echo "hello world
minishell$ echo 'unclosed single quote

# Invalid operators & redirections (sets $? to 2)
minishell$ | ls
minishell$ ls | | wc -l
minishell$ ls >
minishell$ ls > | wc

# Quote concatenation & nested quotes
minishell$ echo "hello"'world'test
minishell$ echo "a'b" '$USER' "$USER"
minishell$ echo $?
```

### 2. Memory & Resource Management (Valgrind Leaks & FDs)

The program has been strictly checked for `memory leaks` and `file descriptor leaks` in both success and error scenarios.

```bash
# Basic leak check
valgrind --leak-check=full ./minishell
```

Since the external GNU `readline` library internally leaves allocations reachable upon termination, a custom suppression file (`readline.supp`) is used to filter out third-party library noise, keeping the focus entirely on project code:

```bash
# Track heap memory, child processes, and unclosed file descriptors:
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --suppressions=readline.supp ./minishell

# Expected Output:
# "All heap blocks were freed -- no leaks are possible"
# "FILE DESCRIPTORS: 3 open at exit (stdin, stdout, stderr)."
```

---

## 📚 Resources

### Documentation & References

* [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — Official reference for quoting rules, shell grammar, expansion order, and exit codes.
* [GNU Readline Library Manual](https://tiswww.case.edu/php/chet/readline/rltop.html) — Documentation for `readline`, `add_history`, and signal display helpers.
* Linux / macOS Man Pages: `man 2 fork`, `man 2 execve`, `man 2 waitpid`, `man 2 pipe`, `man 2 dup2`, `man 2 access`, `man 7 signal`.

### AI Usage Disclosure

All project logic, data structures, and source code were independently designed and implemented by the authors. In accordance with the 42 curriculum guidelines, AI tools were used exclusively as a supplementary research and study aid for:
* **Literature & Source Summarization:** Condensing extensive technical documentation (such as sections of the GNU Bash Reference Manual and POSIX specifications) into structured study notes.
* **Conceptual Reflection:** Reviewing theoretical systems-programming concepts (such as lexical state machines, UNIX process lifecycles, and file descriptor behavior) and cross-checking edge-case assumptions against standard `bash` behavior.
* **Documentation Formatting:** Structuring project notes and formatting the `README.md`.

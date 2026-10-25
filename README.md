_This project has been created as part of the 42 curriculum by **pabartoc**, **esttina** (42 Heilbronn)._


## 🚀 Usage & Local Testing

The program accepts a ...

```bash
# Passing argument:
./minishell
```

### 1. Error Handling Tests (Edge-Cases)

```bash
comming soon
```

### 2. Memory & Resource Management (Valgrind Leaks & FDs)
The program has been strictly checked for `memory leaks` and `file descriptor leaks` in both success and error scenarios. 
```bash
# Test with a single string (Checks if ft_split is freed correctly)
valgrind --leak-check=full ./minishell
```
Since the external GNU `readline` library internally leaves allocations reachable upon termination, a custom suppression file (`readline.supp`) is used to filter out third-party library noise, keeping the focus entirely on project code.
```bash
# Or run the following command to track heap memory, child processes, and unclosed file descriptors:
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --suppressions=readline.supp ./minishell

# Expected Output:
 "All heap blocks were freed -- no leaks are possible"
 "FILE DESCRIPTORS: 3 open at exit (stdin, stdout, stderr)."
```

---




# Libft - Extended 42 C Library (incl. ft_printf & get_next_line)

*This project has been created as part of the 42 curriculum by pabartoc.*

## 📝 What I Learned
Building and extending this library from scratch was a deep dive into the core fundamentals of C programming. Throughout these projects, I learned how to:
- Manage dynamic memory safely using `malloc` and `free` to prevent leaks.
- Manipulate memory and strings at the byte level using pointers.
- Build, traverse, and free complex data structures, specifically singly linked lists.
- Understand the underlying mechanics of standard C library (`libc`) functions by recreating them.
- Work with **variadic functions** (`<stdarg.h>`) to build a custom `ft_printf`.
- Use **static variables** and manage multiple file descriptors simultaneously in `get_next_line`.
- Write robust `Makefile` scripts to automate the compilation of a modular, multi-directory project cleanly and efficiently.

## 🏫 Description
**Libft** is the foundational project of the 42 curriculum. C programming can be quite tedious without access to highly useful standard functions. This repository contains my custom C library (`libft.a`), expanded with **`ft_printf`** and **`get_next_line`** from Circle 01 so that all essential utilities are available in a single static archive for future 42 projects.

It consists of five main parts:
1. Re-implementations of standard `libc` functions.
2. Additional utility functions for string manipulation and output formatting.
3. Functions to manage and manipulate singly linked lists (`t_list`).
4. **`ft_printf`**: Formatted output conversion using variadic arguments.
5. **`get_next_line`**: Line-by-line reading from one or multiple file descriptors.

## 🏗️ Project Structure
This repository is organized in a modular C project structure:

- `inc/`: Contains the main header file (`libft.h`), which also includes the headers for `ft_printf` and `get_next_line`.
- `src/`: Contains all the core `libft` source code (`.c` files).
- `ft_printf/`: Contains the complete `ft_printf` module (`inc/ft_printf.h`, `src/*.c`, and its standalone `Makefile`).
- `get_next_line/`: Contains the mandatory and bonus source/header files for `get_next_line`.
- `obj/`: Created automatically during compilation to store all compiled object files (`.o`), keeping the root directory clean.
- `Makefile`: The root build script used to compile all modules into a single `libft.a`.

## 🏗️ Repository Structure

```text
libft/
├── ft_printf/
│   ├── inc/
│   │   └── ft_printf.h
│   ├── src/
│   │   ├── ft_printf.c
│   │   └── ft_printf_add_ons.c
│   ├── Makefile
│   └── README.md
├── get_next_line/
│   ├── README.md
│   ├── get_next_line.c
│   ├── get_next_line.h
│   ├── get_next_line_bonus.c
│   ├── get_next_line_bonus.h
│   ├── get_next_line_utils.c
│   └── get_next_line_utils_bonus.c
├── inc/
│   └── libft.h
├── src/
│   ├── ft_atoi.c
│   ├── ft_bzero.c
│   ├── ft_calloc.c
│   ├── ft_isalnum.c
│   ├── ft_isalpha.c
│   ├── ft_isascii.c
│   ├── ft_isdigit.c
│   ├── ft_isprint.c
│   ├── ft_itoa.c
│   ├── ft_lstadd_back.c
│   ├── ft_lstadd_front.c
│   ├── ft_lstclear.c
│   ├── ft_lstdelone.c
│   ├── ft_lstiter.c
│   ├── ft_lstlast.c
│   ├── ft_lstmap.c
│   ├── ft_lstnew.c
│   ├── ft_lstsize.c
│   ├── ft_memchr.c
│   ├── ft_memcmp.c
│   ├── ft_memcpy.c
│   ├── ft_memmove.c
│   ├── ft_memset.c
│   ├── ft_putchar_fd.c
│   ├── ft_putendl_fd.c
│   ├── ft_putnbr_fd.c
│   ├── ft_putstr_fd.c
│   ├── ft_split.c
│   ├── ft_strchr.c
│   ├── ft_strdup.c
│   ├── ft_striteri.c
│   ├── ft_strjoin.c
│   ├── ft_strlcat.c
│   ├── ft_strlcpy.c
│   ├── ft_strlen.c
│   ├── ft_strmapi.c
│   ├── ft_strncmp.c
│   ├── ft_strnstr.c
│   ├── ft_strrchr.c
│   ├── ft_strtrim.c
│   ├── ft_substr.c
│   ├── ft_tolower.c
│   └── ft_toupper.c
├── .gitignore
├── Makefile
└── README.md
```

## Instructions

To use this library in your own projects, you need to download and compile it first.

### 👨🏻‍🔧 Installation
Clone the repository:
```bash
git clone https://github.com/pyoskel/libft.git libft
```

### Compilation
Run the following command at the root of the repository to compile the complete library (`libft`, `ft_printf`, and `get_next_line`):
```bash
make
```
This will generate the `libft.a` static library file.

### 🛠️ Makefile Rules
- `make` or `make all`: Compiles all modules into the library `libft.a`.
- `make clean`: Removes the generated object files (`.o`) and the `obj/` directory.
- `make fclean`: Performs `make clean` and also removes the compiled `libft.a` library.
- `make re`: Performs `make fclean` followed by `make all` (rebuilds the library from scratch).

### 🚀 Usage
To use the library (including `ft_printf` and `get_next_line`) in your project, simply include `libft.h` in your C files.

#### 1. Example (`main.c` in the root directory):
```c
#include "libft.h"
#include <fcntl.h>

int main(void)
{
    char *str;
    char *line;
    int   fd;

    // 1. Test: libft + ft_printf
    str = ft_strdup("Hello from the combined Libft!");
    ft_printf("String: %s (Length: %d)\n\n", str, (int)ft_strlen(str));
    free(str);

    // 2. Test: get_next_line + ft_printf (reads the first 11 lines of the Makefile)
    fd = open("Makefile", O_RDONLY);
    if (fd < 0)
        return (1);
    for (int i = 1; i <= 11; i++)
    {
        line = get_next_line(fd);
        ft_printf("Zeile %d: %s", i, line);
        free(line);
    }
    close(fd);
    return (0);
}
```

#### 2. Compile and Run:
```bash
# 1. Build the library
make

# 2. Compile main.c with the library
cc -Wall -Wextra -Werror -I inc main.c libft.a -o test_all

# 3. Run the program
./test_all
```

### 🧪 Memory Leak Testing with Valgrind
To verify that all dynamically allocated memory (from `libft` and `get_next_line`) is properly freed without any memory leaks, run your compiled test binary with Valgrind:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test_all
```
If every allocated string and returned line is freed properly, Valgrind will report:
`All heap blocks were freed -- no leaks are possible`.

## 🔬 Resources
- **Documentation:** `man` pages (e.g., `man 3 memcpy`, `man 3 printf`, `man 2 read`, `man 3 stdarg`) were the primary source of truth for understanding the exact behavior of standard functions and system calls.
- **AI Usage:** AI tools were used strictly as an interactive tutor during this project. They were utilized to understand complex concepts like pointer arithmetic, segmentation faults, memory leaks, and to debug `Makefile` configurations. No code was blindly copied; AI was used to explain *why* errors occurred so I could fix them myself, adhering to the 42 foundational learning approach.

---

## 📋 Detailed Function List

### 🔎 Part 1: Libc Functions
Functions that mimic the behavior of the standard C library.

**Character Classification & Conversion:**

| Function | Description |
|----------|-------------|
| `ft_isalpha` | Checks for an alphabetic character. |
| `ft_isdigit` | Checks for a digit (0-9). |
| `ft_isalnum` | Checks for an alphanumeric character. |
| `ft_isascii` | Checks whether a character fits into the ASCII character set. |
| `ft_isprint` | Checks for any printable character. |
| `ft_toupper` | Converts a lower-case letter to upper-case. |
| `ft_tolower` | Converts an upper-case letter to lower-case. |

**String Manipulation:**

| Function | Description |
|----------|-------------|
| `ft_strlen` | Calculates the length of a string. |
| `ft_strchr` | Locates the first occurrence of a character in a string. |
| `ft_strrchr` | Locates the last occurrence of a character in a string. |
| `ft_strncmp` | Compares two strings up to a specified number of bytes. |
| `ft_strnstr` | Locates a substring within a string, searching up to a given length. |
| `ft_strlcpy` | Size-bounded string copying. |
| `ft_strlcat` | Size-bounded string concatenation. |
| `ft_strdup` | Duplicates a string, allocating memory for the copy. |

**Memory Manipulation:**

| Function | Description |
|----------|-------------|
| `ft_memset` | Fills a block of memory with a specific byte. |
| `ft_bzero` | Erases the data in a specified block of memory by writing zeros. |
| `ft_memcpy` | Copies a specified number of bytes from one memory area to another. |
| `ft_memmove` | Safely copies bytes between potentially overlapping memory areas. |
| `ft_memchr` | Scans a block of memory for a specific character. |
| `ft_memcmp` | Compares two memory blocks byte by byte. |
| `ft_calloc` | Allocates memory for an array and initializes it to zero. |

**Conversion:**

| Function | Description |
|----------|-------------|
| `ft_atoi` | Converts a string representation of an integer to an `int`. |

### 🔍 Part 2: Additional Functions
Useful utility functions not natively found in the standard C library.

| Function | Description |
|----------|-------------|
| `ft_substr` | Extracts a substring from a string. |
| `ft_strjoin` | Concatenates two strings into a new, dynamically allocated string. |
| `ft_strtrim` | Removes specified characters from the beginning and end of a string. |
| `ft_split` | Splits a string into an array of words based on a delimiter character. |
| `ft_itoa` | Converts an integer into its string representation. |
| `ft_strmapi` | Applies a function to each character of a string, creating a new string. |
| `ft_striteri` | Applies a function to each character of a string, modifying it in place. |
| `ft_putchar_fd` | Outputs a character to a given file descriptor. |
| `ft_putstr_fd` | Outputs a string to a given file descriptor. |
| `ft_putendl_fd` | Outputs a string to a given file descriptor, followed by a newline. |
| `ft_putnbr_fd` | Outputs an integer to a given file descriptor. |

### 🔎 Part 3: Linked List Functions
Functions designed to create, manipulate, and iterate over singly linked lists (`t_list`).

| Function | Description |
|----------|-------------|
| `ft_lstnew` | Creates a new list node. |
| `ft_lstadd_front` | Adds a new node at the beginning of a list. |
| `ft_lstsize` | Counts the number of nodes in a list. |
| `ft_lstlast` | Returns the last node of a list. |
| `ft_lstadd_back` | Adds a new node at the end of a list. |
| `ft_lstdelone` | Frees the memory of a node's content and the node itself. |
| `ft_lstclear` | Deletes and frees a given node and every successive node. |
| `ft_lstiter` | Iterates through a list and applies a function to the content of each node. |
| `ft_lstmap` | Iterates through a list, applies a function to create modified content, and builds a completely new list with the results. |

---

### 🖨️ Part 4: `ft_printf` (Circle 01)
A custom re-implementation of the standard C library `printf` function using variadic arguments (`<stdarg.h>`) and direct `write` system calls without dynamic memory allocation.

```c
int ft_printf(const char *format, ...);
```

| Specifier | Description |
|:---------:|-------------|
| `%c` | Prints a single character (`char`). |
| `%s` | Prints a string (`char *`), safely handling `NULL` as `(null)`. |
| `%p` | Prints a pointer address (`void *`) in hexadecimal format (`0x...`). |
| `%d` / `%i` | Prints a signed decimal integer (base 10), handling `INT_MIN`. |
| `%u` | Prints an unsigned decimal integer (`unsigned int`). |
| `%x` / `%X` | Prints an unsigned integer in hexadecimal (lowercase / uppercase). |
| `%%` | Prints a literal percent sign (`%`). |

---

### 📜 Part 5: `get_next_line` (Circle 01)
A function that reads and returns a single line from a file descriptor (`fd`) on each call, preserving unread remainder data across calls using a `static` buffer. The bonus version is compiled into `libft.a`, supporting simultaneous reading from multiple file descriptors.

```c
char *get_next_line(int fd);
```

| Feature | Description |
|---------|-------------|
| **Line-by-Line Reading** | Reads from a file descriptor or standard input (`stdin`) until `\n` or `EOF` is reached. |
| **Configurable `BUFFER_SIZE`** | Defaults to `42` and can be overridden at compile time via `-D BUFFER_SIZE=n`. |
| **Multi-FD Support (Bonus)** | Tracks up to `MAX_FD` (`1024`) file descriptors simultaneously in a 2D static buffer without mixing up reading states. |
| **Error & Memory Safety** | Returns `NULL` on invalid FDs, read errors (`-1`), or `EOF`, freeing any partially allocated line on error. |

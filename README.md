*This project has been created as part of the 42 curriculum by haait-ba.*

# get_next_line

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![42](https://img.shields.io/badge/42-get__next__line-000000?style=for-the-badge&logo=42&logoColor=white)
![Norminette](https://img.shields.io/badge/Norminette-OK-00BABC?style=for-the-badge)

## Description

**get_next_line** is a function that returns one line read from a file descriptor.
Each call returns the next line, so calling it in a loop reads a whole file (or the
standard input) one line at a time.

```c
char	*get_next_line(int fd);
```

| Return value | Meaning |
|---|---|
| the line read | the line, **including** its `\n` (no `\n` only if the file does not end with one) |
| `NULL` | nothing left to read, or an error occurred |

The main concept of the project is the **static variable**: a local variable that keeps
its value between two calls of the same function. It lets the function remember what it
has already read but not yet returned.

### Constraints

- Allowed functions: `read`, `malloc`, `free` only.
- No `libft`, no `lseek`, no global variables.
- Read as little as possible: stop as soon as a `\n` is found, never read the whole
  file first.
- Works for any `BUFFER_SIZE` (1, 42, 9999, 10000000…), with or without `-D BUFFER_SIZE`.
- No memory leaks, no crash.

### Features

- Reads from regular files and from the standard input.
- Handles a last line without `\n`, empty files, and invalid file descriptors.
- **Bonus:** one single static variable, and several file descriptors read at the same
  time without mixing their lines.

## Instructions

### Compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

Without `-D BUFFER_SIZE`, a default value defined in `get_next_line.h` is used.

Bonus:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

### Usage

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

```bash
./a.out                     # reads file.txt
echo -e "a\nb" | ./a.out    # with fd = 0: reads stdin
```

## Algorithm

<!-- TODO: compléter / ajuster quand le code est fini -->

The function keeps a **stash** (static) that holds the characters already read but not
yet returned.

1. **Read:** while the stash has no `\n` and `read` returns more than 0, read
   `BUFFER_SIZE` bytes into a buffer and append them to the stash.
2. **Extract:** copy the stash up to and including the first `\n` (or up to the end if
   there is none) into a new string: this is the line returned.
3. **Clean:** keep only what comes after the `\n` in the stash, for the next call.
4. **End / error:** if `read` returns `-1`, or returns `0` with an empty stash, free the
   stash, set it to `NULL` and return `NULL`.

**Why this approach:** reading stops as soon as a line is complete, so the function never
reads more than needed, and the memory used depends on the line length, not on the file
size.

### Bonus: multiple file descriptors

<!-- TODO: expliquer le choix (tableau de stash indexé par fd, ou liste chaînée) -->

The single static variable is an array of stashes indexed by `fd`, so each file
descriptor keeps its own reading state.

## Project structure

```
get_next_line/
├── get_next_line.c              get_next_line, read / extract / clean
├── get_next_line_utils.c        string helpers (strlen, strchr, strjoin, …)
├── get_next_line.h              prototype, BUFFER_SIZE default
├── get_next_line_bonus.c        multi-fd version
├── get_next_line_utils_bonus.c
└── get_next_line_bonus.h
```

## Tests

- `BUFFER_SIZE` = 1, 2, 42, 9999, 10000000
- Empty file, one line, file without final `\n`, only `\n`, very long lines
- Standard input, invalid fd (`-1`, `42`), closed fd
- Bonus: alternate calls on 3 files
- `valgrind --leak-check=full` and `norminette`

## Resources

- [`read(2)`: Linux manual page](https://man7.org/linux/man-pages/man2/read.2.html)
- [`open(2)`: Linux manual page](https://man7.org/linux/man-pages/man2/open.2.html)
- [Static variables in C: GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)
- [File descriptor: Wikipedia](https://en.wikipedia.org/wiki/File_descriptor)

### Use of AI

I used AI to organise the work plan and to structure this README. The code of the
function was written by me.

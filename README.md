*This project has been created as part of the 42 curriculum by haait-ba.*

# get_next_line

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![42](https://img.shields.io/badge/42-get__next__line-000000?style=for-the-badge&logo=42&logoColor=white)
![Norminette](https://img.shields.io/badge/Norminette-OK-00BABC?style=for-the-badge)

## Description

**get_next_line** returns one line read from a file descriptor. Each call returns the
next line, so calling it in a loop reads a whole file, or the standard input, one line
at a time.

```c
char	*get_next_line(int fd);
```

| Return value | Meaning |
|---|---|
| a line | the next line, **including** its `\n` (no `\n` only for a last line without one) |
| `NULL` | nothing left to read, or an error |

The key concept is the **static variable**: a local variable that keeps its value
between two calls of the same function. It lets `get_next_line` remember what it has
already read but not yet returned.

### Rules

- Allowed functions: `read`, `malloc`, `free`.
- Forbidden: `libft`, `lseek`, global variables.
- Read as little as possible: stop as soon as a `\n` is found.
- Must work with any `BUFFER_SIZE`, with or without `-D BUFFER_SIZE`.
- No leaks, no crash.

### Bonus

- Only **one** static variable.
- Several file descriptors can be read at the same time without mixing their lines.

## Instructions

### Compilation

```bash
# mandatory
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

# bonus
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

Without `-D BUFFER_SIZE`, the default value from `get_next_line.h` is used.

### Example `main.c`

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	fd = 0;
	if (argc > 1)
		fd = open(argv[1], O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	if (fd > 0)
		close(fd);
	return (0);
}
```

```bash
./a.out file.txt              # read a file
printf "one\ntwo" | ./a.out   # read the standard input
```

## Algorithm

`get_next_line` keeps a static **stash**: the characters already read but not yet
returned.

| Step | What happens |
|---|---|
| 1. Read | While the stash has no `\n` and `read` returns more than 0, read `BUFFER_SIZE` bytes and append them to the stash. |
| 2. Extract | Copy the stash up to and including the first `\n` (or to the end if there is none). This is the returned line. |
| 3. Clean | Keep only what comes after the `\n` in the stash, for the next call. |
| 4. Stop | If `read` fails, or the stash is empty at the end of the file: free the stash, set it to `NULL`, return `NULL`. |

**Example** with `BUFFER_SIZE = 4` and the file `"ab\ncd\n"`:

| Call | Read | Stash before extract | Returned | Stash after |
|:---:|---|---|---|---|
| 1 | `"ab\nc"` | `"ab\nc"` | `"ab\n"` | `"c"` |
| 2 | `"d\n"` | `"cd\n"` | `"cd\n"` | `""` |
| 3 | nothing | `""` | `NULL` | freed |

**Why this approach**

- Reading stops as soon as a line is complete, so the function never reads more than needed.
- Memory depends on the length of the line, not on the size of the file.
- With `BUFFER_SIZE = 1` it is slow (one `read` per character) but correct; with a very
  large `BUFFER_SIZE` it uses more memory per call, but works the same way.

**Bonus:** the single static variable is an array of stashes indexed by the file
descriptor (`stash[fd]`), so each fd keeps its own reading state.

## Project structure

All files are at the root of the repository, as required by the subject.

```
get_next_line/
├── get_next_line.h              prototype, default BUFFER_SIZE
├── get_next_line.c              get_next_line: read, extract, clean
├── get_next_line_utils.c        string helpers
├── get_next_line_bonus.h        same, for the bonus
├── get_next_line_bonus.c        multi-fd version (one static array)
├── get_next_line_utils_bonus.c
└── README.md
```

## Tests

| Category | Cases |
|---|---|
| `BUFFER_SIZE` | 1, 2, 42, 9999, 10000000 |
| Files | empty, one line, no final `\n`, only `\n`, very long lines |
| Other fds | standard input, invalid fd (`-1`, `42`), closed fd |
| Bonus | alternate calls on 3 files: fd 3 → 4 → 5 → 3 … |
| Quality | `valgrind --leak-check=full`, `norminette` |

## Resources

- [`read(2)`: Linux manual page](https://man7.org/linux/man-pages/man2/read.2.html)
- [`open(2)`: Linux manual page](https://man7.org/linux/man-pages/man2/open.2.html)
- [Static variables in C: GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)
- [File descriptor: Wikipedia](https://en.wikipedia.org/wiki/File_descriptor)

### Use of AI

I used AI to organise the work plan and to structure this README. The code of the
function was written by me.

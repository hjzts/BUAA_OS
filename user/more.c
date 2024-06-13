#include <lib.h>

// Option values of [-h height] and [-w width], respectively.
int h = 25;
int w = 80;

const int INT_MAX = 2147483647;
char buf[8192];
char out[8192];
char lines[8192];
int index = 0;

// Overview:
//   Interpret integer in range (0, 2^31) in the string 'arg' when handling option 'argname'.
//
// Post-Condition:
//   Return interpreted integer on success.
//   Print error message and halt on error.
int atoi(char* argname, char* arg)
{
    int r = 0;

    while (*arg != '\0') {
        if (*arg < '0' || *arg > '9') {
            user_panic("%s: invalid argument", argname);
        }
        if (r > (INT_MAX - (*arg - '0')) / 10) {
            user_panic("%s: invalid argument", argname);
        }
        r = r * 10 + (*arg - '0');
        arg++;
    }

    if (r <= 0) {
        user_panic("%s: invalid argument", argname);
    }

    return r;
}

int readline()
{
    // cong line_num kaishi du yi hang
    int n = 0;
    for (int i = index, j = 0;; i++, j++) {
        if (j == w) {
            j++;
            lines[j] = '\n';
            break;
        }
        lines[j] = buf[i];
        n++;
        if (buf[i] == '\0')
            break;
        if (buf[i] == '\n' || buf[i] == '\r')
            break;
    }
    index += n;
    // printf("n %d\n",n);
    return n;
}
int more_page(int f)
{
    int i = index;
    int line = 0;
    for (;;) {
        if (line > h)
            break;
        int len = readline();
        if (len == 0) {
            break;
        }
        write(1, lines, len);
        line++;
    }
    return line;
}
int more_line(int f)
{
    int len = readline();
    int r = write(1, lines, len);
    return r;
    // if (r < len) {
    //     user_panic("more_line less write less");
    // }
}

int more(int f)
{
    // printf("222\n");
    long n;
    int r;
    int i;
    int line = 1;
    n = read(f, buf, (long)sizeof buf);
    // while ((n = read(f, buf, (long)sizeof buf)) > 0)
    for (i = 0; i < n; i++) {
        if (line > h) {
            return i;
            break;
        }
        int len = readline();
        i += len;
        line++;
        debugk_user("\t\tline: %d index: %d\n", line, index);
        write(1, lines, len);
    }
    return i;
}

int main(int argc, char** argv)
{
    // Handle arguments.
    for (int i = 1; i < argc; i++) {
        // Handle option [-h height].
        if (strcmp(argv[i], "-h") == 0) {
            if (i + 1 >= argc) {
                user_panic("%s: argument not found", argv[i]);
            }
            h = atoi(argv[i], argv[i + 1]);
            i++;
            continue;
        }

        // Handle option [-w width].
        // You may take [-h height] as an example.
        /* Your code here. (1/6) */
        if (strcmp(argv[i], "-w") == 0) {
            if (i + 1 >= argc) {
                user_panic("%s: argument not found", argv[i]);
            }
            w = atoi(argv[i], argv[i + 1]);
            i++;
            continue;
        }
        int f;
        // Handle argument [file].
        // If 'stdin' is console, no file is opened yet.
        // In this case, open the file and duplicate its file descriptor to 'stdin'.
        if (iscons(0)) {
            /* Your code here. (2/6) */
            f = open(argv[i], O_RDONLY);
            if (f < 0) {
                user_panic("can't open %s: %d", argv[i], f);
            } else {
                // printf("111\n");
                dup(f, 0);
                more(0);
                close(f);
            }
        }

        // Otherwise, there are more than one files in the arguments.
        // Print the corresponding error message and halt.
        /* Your code here. (3/6) */
        else {
            user_panic("too much arguments");
        }
    }

    // No file is opened. Print the corresponding error message and halt.
    if (iscons(0)) {
        /* Your code here. (4/6) */
        user_panic("bad usage");
    }

    // Copy 'stdin' to 'stdout' when 'stdout' is not console.
    if (!iscons(1)) {
        /* Your code here. (5/6) */
        dup(0, 1);
        return 0;
    }

    // Handle interactive commands. You may use 'syscall_cgetc()' to get the console input.
    // You can define variables and functions other than given ones for your convenience.
    /* Your code here. (6/6) */
    char ch;
    int i = 0;
    while (1) {
        ch = syscall_cgetc();
        debugk_user("ch ::: {%c}", ch);
        if (ch == 'q') {
            return 0;
        } else if (ch == ' ') {
            more_page(0);
        } else if (ch == '\r' || ch == '\n') {
            int r = more_line(0);
            if (r == 0) {
                break;
            }
        } else {
            // more(0);
            // break;
            continue;
        }
    }
    // while ((ch = syscall_cgetc()) != '\n')
    // {
    // 	buf[i] = ch;
    // 	i++;
    // }
    // buf[i] = '\0';
    // more(0, argv[i]);
    return 0;
}

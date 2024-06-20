#include <args.h>
#include <lib.h>
// 需要保证这两个变量只在sh中fork的关于每一行的进程在使用，其他进程使用了也没用
// 额，子进程可以读还是可以的
int is_first_cmd = 1;
int condition = 0;
int is_and = 0;
int is_or = 0;
// 有运行的是设置为1
#define WHITESPACE " \t\r\n"
#define SYMBOLS "<|>&;()`\""

/* Overview:
 *   Parse the next token from the string at s.
 *
 * Post-Condition:
 *   Set '*p1' to the beginning of the token and '*p2' to just past the token.
 *   token: [*p1, *p2)
 *   Return:
 *     - 0 if the end of string is reached.
 *     - '<' for < (stdin redirection).
 *     - '>' for > (stdout redirection).
 *     - '|' for | (pipe).
 *     - '+' for >> (stdout append redirect)
 *     - 'a' for && (and , command1 && command2, command2 is executed if and only if command1 returns 0)
 *     - 'o' for || (or , command1 || command2, command2 is executed if and only if command1 returns a non-zero value)
 *     - 'w' for a word (command, argument, or file name).
 *     - 's' for a string ("" a string in the quote symbol)
 *
 *   The buffer is modified to turn the spaces after words into zero bytes ('\0'), so that the
 *   returned token is a null-terminated string.空字符结尾的字符串
 */
int _gettoken(char* s, char** p1, char** p2)
{
    *p1 = 0;
    *p2 = 0;
    if (s == 0) {
        return 0;
    }

    // *s 是否为 WHITESPAVE中某个char，也就是跳过空白符，同时将结尾符设置为'\0'
    while (strchr(WHITESPACE, *s)) {
        *s++ = 0;
    }
    // 如果跳过空白符后是'\0' ,那就说明字符串读完了
    if (*s == 0) {
        return 0;
    }
    // 引号单独处理
    if (*s == '\"') {
        *s++ = 0;
        *p1 = s;
        while (*s && *s != '\"') {
            s++;
        }
        *s++ = 0;
        *p2 = s;
        while (!strchr(WHITESPACE, *s)) {
            s++;
        }
        // char* tmp = *p1;
        // while (*tmp) {
        //     debugf("{%c} ", *tmp);
        //     tmp++;
        // }
        return 's';
    }
    // 如果*s 是特殊字符
    if (strchr(SYMBOLS, *s)) {
        int t = *s;
        *p1 = s;
        // 先将s本来指向的值置为0，然后指向下一个位置
        *s++ = 0;
        *p2 = s;
        if (t == '>' && *s == '>') {
            *s++ = 0;
            *p2 = s;
            return '+';
        } else if (t == '&' && *s == '&') {
            *s++ = 0;
            *p2 = s;
            return 'a';
        } else if (t == '|' && *s == '|') {
            *s++ = 0;
            *p2 = s;
            return 'o';
        }
        return t;
    }

    // word
    *p1 = s;
    while (*s && !strchr(WHITESPACE SYMBOLS, *s)) {
        s++;
    }
    *p2 = s;
    return 'w';
}

int gettoken(char* s, char** p1)
{
    // 静态变量，值会保留在连续的函数调用之间。
    static int c, nc;
    static char *np1, *np2;

    // 是为了设置在第一次调用时初始化nc, np1,np2的值
    if (s) {
        nc = _gettoken(s, &np1, &np2);
        // debugk_user("IN user/sh.c gettoken(), the local <<nc>> is %c, <<np1>> is %s", nc, np1);
        return 0;
    }
    c = nc;
    // p1 保存的是上次读取的参数
    *p1 = np1;
    nc = _gettoken(np2, &np1, &np2);
    return c;
}

#define MAXARGS 128
// parsecmd 中会调用 gettoken，并且其中每次调用第一个参数都是0
// return: argc
int parsecmd(char** argv, int* rightpipe)
{
    // debugk_user("function parsecmd() is called in user/sh.c");
    int argc = 0;
    while (1) {
        char* t;
        int fd, r;
        int c = gettoken(0, &t);
        switch (c) {
        case 0:
            // 结束就返回
            if ((is_and && !condition) || (is_or && condition)) {
                return 0;
            }
            return argc;
        case 'w':
            // 是 word 就保存在 argv 中
            if (argc >= MAXARGS) {
                debugf("too many arguments\n");
                exit();
            }
            argv[argc++] = t;
            break;
        case 's':
            if (argc >= MAXARGS) {
                debugf("too many arguments\n");
                exit();
            }
            debugk_user("IN user/sh.c parsecmd(), the local <<t>> is {%s}", t);
            argv[argc++] = t;
            break;
        case '<':
            if (gettoken(0, &t) != 'w') {
                debugf("syntax error: < not followed by word\n");
                exit();
            }

            // Open 't' for reading, dup it onto fd 0, and then close the original fd.
            // If the 'open' function encounters an error,
            // utilize 'debugf' to print relevant messages,
            // and subsequently terminate the process using 'exit'.

            /* Exercise 6.5: Your code here. (1/3) */
            if ((fd = open(t, O_RDONLY)) < 0) {
                // user_panic("< open failed");
                debugf("failed to open '%s'\n", t);
                exit();
            }
            dup(fd, 0);
            // if ((r = dup(fd, 0)) < 0) {
            //     debugf("failed to duplicate file to <stdin>\n");
            //     exit();
            // }
            close(fd);
            // user_panic("< redirection not implemented");

            break;
        case '>':
            if (gettoken(0, &t) != 'w') {
                debugf("syntax error: > not followed by word\n");
                exit();
            }
            // Open 't' for writing, create it if not exist and trunc it if exist, dup
            // it onto fd 1, and then close the original fd.
            // If the 'open' function encounters an error,
            // utilize 'debugf' to print relevant messages,
            // and subsequently terminate the process using 'exit'.
            /* Exercise 6.5: Your code here. (2/3) */
            if ((fd = open(t, O_WRONLY)) < 0) {
                // user_panic("> open failed");
                debugf("failed to open '%s'\n", t);
                exit();
            }
            dup(fd, 1);
            // if ((r = dup(fd, 1)) < 0) {
            //     debugf("failed to duplicate file to <stdout>\n");
            //     exit();
            // }
            close(fd);
            // user_panic("> redirection not implemented");

            break;
        case '|':;
            /*
             * First, allocate a pipe.
             * Then fork, set '*rightpipe' to the returned child envid or zero.
             * The child runs the right side of the pipe:
             * - dup the read end of the pipe onto 0
             * - close the read end of the pipe
             * - close the write end of the pipe
             * - and 'return parsecmd(argv, rightpipe)' again, to parse the rest of the
             *   command line.
             * The parent runs the left side of the pipe:
             * - dup the write end of the pipe onto 1
             * - close the write end of the pipe
             * - close the read end of the pipe
             * - and 'return argc', to execute the left of the pipeline.
             */
            // a label can only be part of a statement and a declaration is not a statement
            int p[2];
            /* Exercise 6.5: Your code here. (3/3) */
            pipe(p);
            // if ((r = pipe(p)) < 0) {
            //     debugf("failed to create pipe\n");
            //     exit();
            // }
            if ((*rightpipe = fork()) == 0) {
                dup(p[0], 0);
                close(p[0]);
                close(p[1]);
                return parsecmd(argv, rightpipe);
            } else {
                dup(p[1], 1);
                close(p[1]);
                close(p[0]);
                return argc;
            }
            // user_panic("| not implemented");
            break;
        case '\"':
            debugk_user("IN user/sh.c parsecmd(), now the local variable <<c>> is \"");
            break;
        case '`':
            debugk_user("IN user/sh.c parsecmd(), now the local variable <<c>> is `");
            break;
        case ';':;
            debugk_user("IN user/sh.c parsecmd(), now the local variable <<c>> is ;");
            // 创建一个子进程来执行左边的命令，返回的left是子进程的env_id
            int left = fork();
            if (left > 0) {
                // 让父进程暂停，直到子进程结束
                wait(left);
                // int exit_code = wait(left);
                // debugk_user("IN user/sh.c parsecmd(), the local variable <<exit_code>> is %d", exit_code);
                return parsecmd(argv, rightpipe);
            } else {
                return argc;
            }
            break;
        case '(':
            break;
        case ')':
            break;
        case '+':
            // Append redirect
            debugk_user("IN user/sh.c parsecmd(), now the local variable <<c>> is >> ");
            break;
        case 'a':;
            // and
            // debugk_user("IN user/sh.c parsecmd(), now the local variable <<c>> is &&");
            left = fork();
            if (left > 0) {
                // 让父进程暂停，直到子进程结束
                int exit_code = wait(left);
                debugk_user("IN user/sh.c parsecmd(), the local variable <<exit_code>> is %d of env %x", exit_code, syscall_getenvid());
                if (is_first_cmd) {
                    is_first_cmd = 0;
                    condition = exit_code == 0;
                } else if (!condition) {
                    condition = exit_code == 0;
                }
                is_and = 1;
                is_or = 0;
                debugk_user("IN user/sh.c parsecmd(), the env is %x, and the condition is %d\n", syscall_getenvid(), condition);
                syscall_set_condition(0, condition);
                debugk_user("IN user/sh.c parsecmd(), the local variable <<condition>> is %d", condition);
                return parsecmd(argv, rightpipe);
            } else {
                // debugk_user("IN user/sh.c parsecmd(), the local variable <<condition>> is %d", condition);
                return argc;
            }
            // command1 && command2, command2 is executed if and only if command1 returns 0
            break;
        case 'o':
            // or
            debugk_user("IN user/sh.c parsecmd(), now the local variable <<c>> is ||");
            left = fork();
            if (left > 0) {
                // 让父进程暂停，直到子进程结束
                int exit_code = wait(left);
                debugk_user("IN user/sh.c parsecmd(), the local variable <<exit_code>> is %d of env %x", exit_code, syscall_getenvid());
                if (is_first_cmd) {
                    is_first_cmd = 0;
                    condition = exit_code == 0;
                } else if (!condition) {
                    condition = exit_code == 0;
                }
                is_and = 0;
                is_or = 1;
                debugk_user("IN user/sh.c parsecmd(), the env is %x, and the condition is %d\n", syscall_getenvid(), condition);
                syscall_set_condition(0, !condition);
                debugk_user("IN user/sh.c parsecmd(), the local variable <<condition>> is %d", condition);
                return parsecmd(argv, rightpipe);
            } else {
                // debugk_user("IN user/sh.c parsecmd(), the local variable <<condition>> is %d", condition);
                return argc;
            }
            // command1 || command2, command2 is executed if and only if command1 returns a non-zero value
            break;
        }
    }
    return argc;
}

// *s,也就是读入的命令字符串：buf
void runcmd(char* s)
{
    // debugk_user("function runcmd is called in user/sh.c");
    // 只有第一次调用gettoken，第一个参数*s才不是0，是为了初始化gettoken中的静态变量
    gettoken(s, 0);
    char* argv[MAXARGS];
    int rightpipe = 0;
    int argc = parsecmd(argv, &rightpipe);

    // debugk_user("IN user/sh.c runcmd() the function <<parsecmd>> is called; argc=%d, the argv is :", argc);
    for (int i = 0; i < argc; i++) {
        debugk_user("{%s} ", argv[i]);
    }

    if (argc == 0) {
        return;
    }
    int condition;
    syscall_get_condition(0, &condition);
    debugk_user("IN user/sh.c runcmd(), the env's parent is %x, and the condition is %d\n", syscall_get_parent_envid(), condition);
    if (condition == 0) {
        just_exit();
    }
    argv[argc] = 0;

    // argv[0] 表示 命令本身所代表的二进制文件的名字
    // 最后一个cmd就是这个shell执行的
    int child = spawn(argv[0], argv);
    debugk_user("IN user/sh.c runcmd() , the function <<spawn>> %s: %x", argv[0], child);
    close_all();
    if (child >= 0) {
#ifdef RETURN_VALUE
        int exit_code = wait(child);
        syscall_set_exit_code(syscall_getenvid(), exit_code);
        debugk_user("IN user/sh.c runcmd(), the local variable <<exit_code>> from %x is %d", child, exit_code);
#else
        wait(child);
#endif
    } else {
        debugf("spawn %s: %d\n", argv[0], child);
    }
    if (rightpipe) {
        wait(rightpipe);
    }
    just_exit();
}

void readline(char* buf, u_int n)
{
    int r;
    for (int i = 0; i < n; i++) {
        if ((r = read(0, buf + i, 1)) != 1) {
            if (r < 0) {
                debugf("read error: %d\n", r);
            }
            exit();
        }
        // debugk_user("IN user/sh.c readline(), thel local i is %d",i);
        if (buf[i] == '\b' || buf[i] == 0x7f /*DEL (Delete) 符号*/) {
            // 因为之后会执行i++操作，表示如果已经在buf中有字符，则回退一个，否则留在原地
            if (i > 0) {
                i -= 2;
            } else {
                i = -1;
            }
            if (buf[i] != '\b') {
                // 这个表示将光标左移一位
                printf("\b");
            }
        }
        if (buf[i] == '\r' || buf[i] == '\n') {
            buf[i] = 0;
            return;
        }
    }
    // 理论上应该在上面的'\r' '\n'那里结束，没有结束说明太长了
    debugf("line too long\n");
    // 读完这一行，并将buf[0]置为'\0'，表示为空字符串
    while ((r = read(0, buf, 1)) == 1 && buf[0] != '\r' && buf[0] != '\n') {
        ;
    }
    buf[0] = 0;
}

void process_comments(char* buf, u_int n)
{
    for (int i = 0; i < n; i++) {
        if (buf[i] == '\0')
            return;
        if (buf[i] == '#') {
            buf[i] = '\0';
            return;
        }
    }
}

char buf[1024];

void usage(void)
{
    printf("usage: sh [-ix] [script-file]\n");
    exit();
}

int main(int argc, char** argv)
{
    int r;
    int interactive = iscons(0);
    int echocmds = 0;
    printf("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("::                                                         ::\n");
    printf("::                     MOS Shell 2024                      ::\n");
    // printf("::                  MOS Hugo Shell 2024                    ::\n");
    printf("::                                                         ::\n");
    printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    // printf("            _/    _/  _/    _/    _/_/_/    _/_/             \n");
    // printf("           _/    _/  _/    _/  _/        _/    _/            \n");
    // printf("          _/_/_/_/  _/    _/  _/  _/_/  _/    _/             \n");
    // printf("         _/    _/  _/    _/  _/    _/  _/    _/              \n");
    // printf("        _/    _/    _/_/      _/_/_/    _/_/                 \n");
    ARGBEGIN
    {
    case 'i':
        debugk_user("IN user/sh.c main(), the interactive is set");
        interactive = 1;
        break;
    case 'x':
        debugk_user("IN user/sh.c main(), the echocmds is set");
        echocmds = 1;
        break;
    default:
        usage();
    }
    ARGEND

    // debugk_user("IN user/sh.c main(), the local <<interactive>> is %d, <<echocmds>> is %d", interactive, echocmds);
    // debugk_user("IN user/sh.c main(), the local <<argc>> is %d,  and IN user/sh.c main(), the local <<argv>> is ", argc);
    for (int i = 0; i < argc; i++) {
        debugk_user("{%s} ", argv[i]);
    }
    if (argc > 1) {
        usage();
    }
    if (argc == 1) {
        // with script file,仅仅只是将标准输入修改为argv[0]这个脚本文件作为输入
        close(0);
        if ((r = open(argv[0], O_RDONLY)) < 0) {
            user_panic("open %s: %d", argv[0], r);
        }
        user_assert(r == 0);
    }
    for (;;) {
        if (interactive) {
            printf("\n$ ");
        }
        readline(buf, sizeof buf);
        is_first_cmd = 1;
        // debugk_user("IN sh.c main() the local variable <<buf>> is %s", buf);
        process_comments(buf, sizeof buf);
        // debugk_user("IN sh.c main() the local variable <<buf>> after process_comments is %s", buf);
        if (buf[0] == '#') {
            continue;
        }
        if (echocmds) {
            // 表示回显输入字符串
            printf("# %s\n", buf);
        }
        // fork 一个子进程来runcmd
        if ((r = fork()) < 0) {
            user_panic("fork: %d", r);
        }
        if (r == 0) {
            // 子进程负责runcmd，对那一行进行处理
            runcmd(buf);
            exit();
        } else {
            // 父进程，也就是那个shell进程直接忙等
            // TODO:想把这个优化一下
            wait(r);
        }
    }
    return 0;
}

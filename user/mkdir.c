#include <lib.h>

int flag[256];

int mkdir(char* dirPath)
{
    int f;
    if ((f = open(dirPath, O_EXCL)) == -E_FILE_EXISTS) {
        printf("mkdir: cannot create directory %s: File exists", dirPath);
        return 1;
    }
    int size = strlen(dirPath);
    int index = -1;
    for (int i = size - 1; i >= 0; i--) {
        if (dirPath[i] == '/') {
            index = i;
            break;
        }
    }
    // 检查父目录是否存在
    if (index > 0 && !flag['p']) {
        dirPath[index] = '\0';
        int f_tmp = open(dirPath, O_EXCL);
        dirPath[index] = '/';
        if (f_tmp == -E_NOT_FOUND) {
            printf("mkdir: cannot create directory %s: No such file or directory\n", dirPath);
            return 1;
        }
    }
    f = open(dirPath, O_CREAT | O_MKDIR | O_IGNORE_ERROR);
    close(f);
    return 0;
}

void usage(void)
{
    printf("usage: mkdir [-p] [dir]\n");
#ifdef RETURN_VALUE
    exit_with_exit_code(1);
#else
    exit();
#endif
}
int main(int argc, char** argv)
{
    debugk_user("the argc is %d, ", argc);
    for (int i = 0; i < argc; i++) {
        debugk_user("argv [%d] is %s", i, argv[i]);
    }
    ARGBEGIN
    {
    default:
        usage();
    case 'p':
        flag[(u_char)ARGC()]++;
        break;
    }
    ARGEND

    if (argc == 0) {
        usage();
        return 1;
    }
    for (int i = 0; i < argc; i++) {
        return mkdir(argv[i]);
    }
}
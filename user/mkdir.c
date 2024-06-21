#include <lib.h>

int flag[256];

int mkdir(char* dirPath)
{
    int f;
    if ((f = open(dirPath, O_EXCL)) < 0) {
        debugf("the dir is already exists");
        return 1;
    }
    if (flag['p']) {
        f = open(dirPath, O_CREAT | O_MKDIR | O_IGNORE_ERROR);
    } else {
        if ((f = open(dirPath, O_CREAT | O_MKDIR)) < 0) {
            return 1;
        }
    }
    return 0;
}

void usage(void)
{
    printf("usage: mkdir [-p] [dir]\n");
    exit();
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
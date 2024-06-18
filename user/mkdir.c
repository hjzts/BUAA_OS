#include <lib.h>

int flag[256];

void mkdir(char* dirPath)
{
    int f;
    if ((f = open(dirPath, O_EXCL)) == 0) {
        
    }
    if ((f = open(dirPath, O_MKDIR)) < 0) {
    }
}

void usage(void)
{
    printf("usage: mkdir [-] [dir]\n");
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

    for (int i = 0; i < argc; i++) {
        mkdir(argv[i]);
    }

    return 0;
}
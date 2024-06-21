#include <lib.h>

int flag[256];

int rm(char* path)
{
    // file path OR dir path
    int fd, n;
    struct File f;
    if ((fd = open(path, O_EXCL)) == 0) {
        if (flag['f'])
            return 0;
        debugf("rm: cannot remove %s: No such file or directory", path);
        // debugf("the file or dir does not exists");
        return 1;
    }
    while ((n = readn(fd, &f, sizeof f)) == sizeof f) {
        if (f.f_name[0]) {
            if (f.f_type == FTYPE_DIR) {
                if (flag['r']) {
                    f.f_name[0] = '\0';
                } else {
                    debugf("rm: cannot remove %s: Is a directory", path);
                    return 1;
                }
            } else {
                f.f_name[0] = '\0';
            }
        }
    }
    return 0;
}

void usage(void)
{
    printf("usage: rm [-rf] [file|dir]\n");
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
    case 'r':
    case 'f':
        flag[(u_char)ARGC()]++;
        break;
    }
    ARGEND

    if (argc == 0) {
        usage();
        return 1;
    }
    for (int i = 0; i < argc; i++) {
        return rm(argv[i]);
    }
}
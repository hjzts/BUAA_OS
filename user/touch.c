#include <lib.h>
int touch(char* file)
{
    // nonexistent/dir/a.txt
    int f;
    // 表示文件已经存在，按照样例应该不会出现这种情况
    if ((f = open(file, O_EXCL)) == -E_FILE_EXISTS) {
        // debugf("the file is already exists\n");
        close(f);
        return 1;
    }
    if ((f = open(file, O_CREAT)) < 0) {
        debugf("touch: cannot touch '%s': No such file or directory\n",file);
    } else {
        close(f);
    }
    return 0;
}
void usage(void)
{
    printf("usage: touch [file]\n");
    exit();
}
int main(int argc, char** argv)
{
    debugk_user("the argc is %d, ", argc);
    for (int i = 0; i < argc; i++) {
        debugk_user("argv [%d] is %s", i, argv[i]);
    }
    if (argc == 2) {
        // 目前只可能是 touch <file>，不考虑参数和有多个file的情况
        return touch(argv[1]);
    } else {
        return 1;
    }
}
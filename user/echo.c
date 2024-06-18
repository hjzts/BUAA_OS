#include <lib.h>

int main(int argc, char** argv)
{
    debugk_user("PROGRAM user/echo.c is used");
    debugk_user("the argc is %d, ", argc);
    for (int i = 0; i < argc; i++) {
        debugk_user("argv [%d] is %s", i, argv[i]);
    }
    int i, nflag;

    nflag = 0;
    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        nflag = 1;
        argc--;
        argv++;
    }
    for (i = 1; i < argc; i++) {
        if (i > 1) {
            printf(" ");
        }
        printf("%s", argv[i]);
    }
    if (!nflag) {
        printf("\n");
    }
    return 0;
}

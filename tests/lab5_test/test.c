#include "lib.h"

static char* msg = "This is the NEW message of the day!\n\n";
static char* diff_msg = "This is a different massage of the day!\n\n";

int main()
{
    int r;
    int fdnum;
    char buf[512];
    int n;

    

    if ((r = open("/newmotd", O_RDWR)) < 0) {
        user_panic("open /newmotd: %d", r);
    }
    fdnum = r;
    debugf("open is good\n");

    if ((n = read(fdnum, buf, 511)) < 0) {
        user_panic("read /newmotd: %d", r);
    }
    if (strcmp(buf, diff_msg) != 0) {
        user_panic("read returned wrong data");
    }
    debugf("read is good\n");

    int id;

    if ((id = fork()) == 0) {
        if ((n = read(fdnum, buf, 511)) < 0) {
            user_panic("child read /newmotd: %d", r);
        }
        if (strcmp(buf, diff_msg) != 0) {
            user_panic("child read returned wrong data");
        }
        debugf("child read is good && child_fd == %d\n", r);
        struct Fd* fdd;
        fd_lookup(r, &fdd);
        debugf("child_fd's offset == %d\n", fdd->fd_offset);
    } else {
        if ((n = read(fdnum, buf, 511)) < 0) {
            user_panic("father read /newmotd: %d", r);
        }
        if (strcmp(buf, diff_msg) != 0) {
            user_panic("father read returned wrong data");
        }
        debugf("father read is good && father_fd == %d\n", r);
        struct Fd* fdd;
        fd_lookup(r, &fdd);
        debugf("father_fd's offset == %d\n", fdd->fd_offset);
    }

    return 0;
}
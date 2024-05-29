#ifndef _USER_FD_H_
#define _USER_FD_H_ 1

#include <fs.h>

#define debug 0

#define MAXFD 32
#define FILEBASE 0x60000000
#define FDTABLE (FILEBASE - PDMAP)

#define INDEX2FD(i) (FDTABLE + (i)*PTMAP)
#define INDEX2DATA(i) (FILEBASE + (i)*PDMAP)

// pre-declare for forward references
struct Fd;
struct Stat;
struct Dev;

// Device struct:
// It is used to read and write data from corresponding device.
// We can use the five functions to handle data.
// There are three devices in this OS: file, console and pipe.
struct Dev {
	int dev_id;
	char *dev_name;
	int (*dev_read)(struct Fd *, void *, u_int, u_int);
	int (*dev_write)(struct Fd *, const void *, u_int, u_int);
	int (*dev_close)(struct Fd *);
	int (*dev_stat)(struct Fd *, struct Stat *);
	int (*dev_seek)(struct Fd *, u_int);
};

// file descriptor
struct Fd {
	u_int fd_dev_id;	// 外设的id
	// 用户是用fd.c的用户接口是，不同的dev_id会调取不同的文件服务函数
	// fd_dev_id的取值可以是devfile.dev_id "f"  或者是 devcons.dev_id "c"
	u_int fd_offset;	// 读写的偏移量
	// 在file_read、file_write会改变这个偏移量
	// 在seek()时也会修改
	// offset会被用来找起始filebno文件块号。
	u_int fd_omode;		// 打开方式，包括只读、只写、读写
	// serve_open是会进行修改，read和write时会用到
};

// State
struct Stat {
	char st_name[MAXNAMELEN];
	u_int st_size;
	u_int st_isdir;
	struct Dev *st_dev;
	u_int st_mode;
};

// file descriptor + file
// 为了让Fd*类型的结构体可以存储更多信息，常常用来强转
struct Filefd {
	struct Fd f_fd;		// file descriptor 文件描述符
	u_int f_fileid;		// 文件的id
	// 会用来索引opentab[]中对应的open控制块
	struct File f_file;	// 这个文件描述符对应的文件控制块
};

int fd_alloc(struct Fd **fd);
int fd_lookup(int fdnum, struct Fd **fd);
void *fd2data(struct Fd *);
int fd2num(struct Fd *);
int dev_lookup(int dev_id, struct Dev **dev);
int num2fd(int fd);
extern struct Dev devcons;
extern struct Dev devfile;
extern struct Dev devpipe;

#endif

#ifndef _FS_H_
#define _FS_H_ 1

#include <stdint.h>

// File nodes (both in-memory and on-disk)

// Bytes per file system block - same as page size
#define BLOCK_SIZE PAGE_SIZE // 就是PAGE_SIZE 4096Byte 4KB
#define BLOCK_SIZE_BIT (BLOCK_SIZE * 8)

// Maximum size of a filename (a single path component), including null
#define MAXNAMELEN 128

// Maximum size of a complete pathname, including null
#define MAXPATHLEN 1024

// Number of (direct) block pointers in a File descriptor
#define NDIRECT 10
#define NINDIRECT (BLOCK_SIZE / 4)

#define MAXFILESIZE (NINDIRECT * BLOCK_SIZE)

#define FILE_STRUCT_SIZE 256

#define FMODE_R 0x4
#define FMODE_W 0x2
#define FMODE_X 0x1
#define FMODE_RW 0x6
#define FMODE_ALL 0x7

#define STMODE2FMODE(st_mode) (((st_mode) >> 6) & FMODE_ALL)

struct File {
    char f_name[MAXNAMELEN]; // filename		文件名称，最大长度为128
    uint32_t f_size; // file size in bytes	文件的大小，单位为字节
    uint32_t f_type; // file type			文件类型，有普通文件FTYPE_REG和目录FTYPE_DIR两种
    uint32_t f_direct[NDIRECT]; // 文件的直接指针，每个文件控制块有10个直接指针，用来记录文件的数据块在磁盘上的位置
                                // 每个磁盘块的大小为4KB，也就是这10个直接指针能够表示最大40KB的文件
    uint32_t f_indirect; // 文件大于40KB时，需要用到间接指针。
	// ((int*)(disk[dirf->f_indirect].data))[i]
    struct File* f_dir; // the pointer to the dir where this file is in, valid only in memory.指向文件所属的文件目录
    uint32_t f_mode;
    char f_pad[FILE_STRUCT_SIZE - MAXNAMELEN - (4 + NDIRECT) * 4 - sizeof(void*)];
    //	是为了让整数个文件结构体占用一个磁盘块，填充结构体中剩下的字节
} __attribute__((aligned(4), packed));

#define FILE2BLK (BLOCK_SIZE / sizeof(struct File))	// 也就是每一个块有多少个文件控制块，向下取整，这里是16

// File types
#define FTYPE_REG 0 // Regular file
#define FTYPE_DIR 1 // Directory

// File system super-block (both in-memory and on-disk)

#define FS_MAGIC 0x68286097 // Everyone's favorite OS class

struct Super {
    uint32_t s_magic; // Magic number: FS_MAGIC
    uint32_t s_nblocks; // Total number of blocks on disk
    struct File s_root; // Root directory node
};

#endif // _FS_H_

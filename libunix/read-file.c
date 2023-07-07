#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libunix.h"

// allocate buffer, read entire file into it, return it.   
// buffer is zero padded to a multiple of 4.
//
//  - <size> = exact nbytes of file.
//  - for allocation: round up allocated size to 4-byte multiple, pad
//    buffer with 0s. 
//
// fatal error: open/read of <name> fails.
//   - make sure to check all system calls for errors.
//   - make sure to close the file descriptor (this will
//     matter for later labs).
// 
void *read_file(unsigned *size, const char *name) {
    // How: 
    //    - use stat to get the size of the file.
    //    - round up to a multiple of 4.
    //    - allocate a buffer
    //    - zero pads to a multiple of 4.
    //    - read entire file into buffer.  
    //    - make sure any padding bytes have zeros.
    //    - return it. 
    struct stat st;
    int check = stat(name, &st);
    if (check == -1) {
        perror("Error with stat");
    }
    *size = st.st_size;
    int buf_size = (st.st_size+4) - (st.st_size+4)%4;
    char *buf = calloc(buf_size, sizeof(char));
    int fd = open(name, O_RDONLY);
    if (fd != -1) {
        read(fd, buf, *size);
    }
    close(fd);
    return buf;
}

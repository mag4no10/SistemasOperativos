#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

/*
#define BUF_SIZE 65536

int cp(const char *to, const char *from)
{
    int fd_to, fd_from;
    char *buf = (char*) malloc(BUF_SIZE* sizeof(char));
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_to);
        close(fd_from);

        return 0; 
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}
/*
#include <iostream>
#define BUF_SIZE 65536

void cp(const char *from, const char*to){
FILE *src, *dst;
size_t in, out;
char *buf = (char*) malloc(BUF_SIZE* sizeof(char));
src = fopen(from, "rb");
if (NULL == src) exit(2);
dst = fopen(to, "wb");
if (dst < 0) exit(3);
while (1) {
    in = fread(buf, sizeof(char), BUF_SIZE, src);
    if (0 == in) break;
    out = fwrite(buf, sizeof(char), in, dst);
    if (0 == out) break;
}
fclose(src);
fclose(dst);
}
*/

int copy(const char* src, const char* dst){

    int fd_src, fd_dst;         // File Descriptors de Source y Destination
    struct stat stat;           // Estructura para stat.h
    uint32_t source, destiny;              // "Buffer"

    fd_src = open(src, O_RDONLY);
    //if (in < 0) {
    //perror("open(src, ...)");
    //exit(EXIT_FAILURE);
    //}

    if (fstat(fd_src, &stat)) {
        perror("fstat(in, ...)");
        exit(EXIT_FAILURE);
    }
    
    fd_dst = open(dst, O_CREAT|O_WRONLY|O_TRUNC, 0644); 
    //if (out < 0) {
    //perror("open(dst, ...)");
    //exit(EXIT_FAILURE);
    //}
    source = stat.st_size;

    do {
        destiny = copy_file_range(fd_src, NULL, fd_dst, NULL, source, 0);
        //if (n < 1) {
        //    perror("copy_file_range(...)");
        //    exit(EXIT_FAILURE);
        //}   
        source -= destiny;
    } 
    while(source > 0 && destiny > 0);
  
  
  close(fd_src);
  close(fd_dst);
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    char* mode     = argv[1]; 
    char* src_path = argv[2];
    char* dst_path = argv[3];

    int error = copy(src_path,dst_path);
    std::cout << "Finished copy from " << src_path << " to " << dst_path <<
    std::endl;
    return 0;
}
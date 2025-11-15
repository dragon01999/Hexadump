#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define MIN_ARGS 2
#define MAX_ARGS 3
void print_hex(char *buff, int byte_count, int *offset)
{
    int k = 0;
    char text[16];
    for (int i = 0; i < byte_count; i++) {
        if (*offset % 16 == 0) {
            if (i > 0)
                printf(" %s", text);
            printf("\n%08x:", *offset);
        }
        if (i % 2 == 0)
            putchar(' ');
        printf("%02x",buff[i]);
        text[(i % 16)] = buff[i];
        *offset += 0x1;
    }
    return;
}

int main(int argc, char **argv) {
    int fd, opt, total_bytes, print_mode;
    char *file;
    size_t bytes_read = 0;
    if (argc < MIN_ARGS || argc > MAX_ARGS) {
        fprintf(stderr, "Usage: ./hexdump filename \n");
        return 1;
    }
    file = argv[1];
    /* Might add extra features in futures thats why using getopt */
    opt = getopt(2, argv, "b");  
    if (opt != -1) {
        switch(opt) {
            case 'b':
                print_mode = 1;
                file = argv[2];
                break;
            case '?':
                fprintf(stderr, "Unkown option. Use -b for binary dump for hex dump default\n");
                exit(EXIT_FAILURE);
            default:
                print_mode = 0;
                fprintf(stderr, "def");
                file = argv[1];
                break;
        }
    }
    fd = open(file, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Couldn't open the file: %s\n", strerror(errno));
        return 1;
    }
//    char *buff = malloc(sizeof(char) * 512);
    char buff[512];
    int offset = 0x0;
    while ((bytes_read = read(fd, buff, sizeof(buff))) > 0) {
        print_hex(buff, bytes_read, &offset);
    }
}

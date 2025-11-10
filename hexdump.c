#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define MIN_ARGS 2
#define MAX_ARGS 3

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
    int buff;
    while ((bytes_read = read(fd, &buff, sizeof(char))) > 0) {
        if (buff == '\n') {
            printf("\n");
            continue;
        }
        printf("%x ", buff);
    }
}
 



    





#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BLUE "\033[94m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define OFF "\033[0m"
#define MIN_ARGS 2
#define MAX_ARGS 3

void print_dump(unsigned char *buff, int byte_count, int *offset, int opt)
{
    unsigned char text[16];
    for (int i = 0; i < byte_count; i++) {
        if (*offset % 16 == 0) {
            if (i > 0) /* dont print before printing values */
                printf(" %s%s%s", CYAN, text, OFF);
            memset(text, 0, sizeof(text)); /* set string buff to 0 to clear it */ 
            if (opt == 'o')
                printf("\n%08o:", *offset);
            else
                printf("\n%08x:", *offset);
        }
        if (i % 2 == 0)
            putchar(' ');
        if (opt == 'o')
            printf("%s%02o%s", GREEN, buff[i], OFF);
        else
            printf("%s%02x%s", GREEN, buff[i], OFF);
        /* if non printable or unecessary use '.' */
        if (buff[i] >= 0x20 && buff[i] <= 0x7e)
            text[(i % 16)] = buff[i];
        else
            text[(i % 16)] = '.';
        *offset += 0x1;
    }
    /* print the final line since loop would have exited */
    printf(" %s%s%s", CYAN, text, OFF);
    return;
}

int main(int argc, char **argv) {
    int fd, opt, total_bytes;
    char *file;
    size_t bytes_read = 0;
    if (argc < MIN_ARGS || argc > MAX_ARGS) {
        fprintf(stderr, "Usage: ./hexdump filename \n");
        return 1;
    }
    file = argv[1];
    /* Might add extra features in futures thats why using getopt */
    opt = getopt(2, argv, "o");  
    if (opt != -1) {
        switch(opt) {
            case 'o':
                file = argv[2];
                break;
            case '?':
                fprintf(stderr, "Unkown option. Use -b for binary dump for hex dump default\n");
                return 1;
            default:
                break;
        }
    }
    fd = open(file, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Couldn't open the file: %s\n", strerror(errno));
        return 1;
    }
    unsigned char buff[1024];
    int offset = 0x0;
    while ((bytes_read = read(fd, buff, sizeof(buff))) > 0) {
        print_dump(buff, bytes_read, &offset, opt);
    }
    if (bytes_read == -1) {
        fprintf(stderr, "Error while reading:%s", strerror(errno));
}

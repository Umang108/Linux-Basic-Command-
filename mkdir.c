#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
        return 1;
    }

    if (mkdir(argv[1], 0755) != 0) {
        printf("Error creating directory");
        return 1;
    }
    
    return 0;
}




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void count_words_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    int word_count = 0;
    char buffer[1024];

    while (fscanf(file, "%s", buffer) != EOF) {
        word_count++;
    }

    fclose(file);
    printf("Word count: %d\n", word_count);
}

void count_files_in_directory(const char *directory_name) {
    DIR *dir = opendir(directory_name);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    int file_count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Count only regular files
            file_count++;
        }
    }

    closedir(dir);
    printf("File count: %d\n", file_count);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("command not found\n");
        return 1;
    }

    if (strcmp(argv[1], "-w") == 0) {
        count_words_in_file(argv[2]);
    } else if (strcmp(argv[1], "-l") == 0) {
        count_files_in_directory(argv[2]);
    } else {
        printf("Invalid option. Use -w for word count or -l for file count.\n");
        return 1;
    }

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define COMMAND_LENGTH 1024
#define ARGS 100

// Function to display help information
void display_help() {
    printf("Simple Shell\n");
    printf("Available commands:\n");
    printf("help: Display this help information\n");
    printf("clear: Clear the terminal screen\n");
    printf("exit: Exit the shell\n");
    printf("You can run any other system command!\n");
}

// Function to clear the terminal
void clear_screen() {
    printf("\033[H\033[J");  // ANSI escape code to clear the screen
}

// Function to check if the command is valid (exists in ./bin/)
int isValidCommand(char *cmd) {
    char path[1024];  // Buffer for the path
    sprintf(path, "/home/anurag/.bin/%s", cmd);  // Construct the full path

    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return 1;  // Command is valid (file exists)
    }
    return 0;  // Command not found
}

// Function to execute a command with optional I/O redirection or piping
void execute_command(char *args[], int input_fd, int output_fd) {
    if (input_fd != 0) {
        dup2(input_fd, STDIN_FILENO);  // Redirect input
        close(input_fd);
    }
    if (output_fd != 1) {
        dup2(output_fd, STDOUT_FILENO);  // Redirect output
        close(output_fd);
    }

    if (execv(args[0], args) == -1) {
        fprintf(stderr, "Command execution failed: %s\n", strerror(errno));
        exit(1);
    }
}

// Function to parse and handle pipes and redirection
void process_command(char *cmd) {
    char *args[ARGS];
    char *input_file = NULL;
    char *output_file = NULL;
    int input_fd = 0;   
    int output_fd = 1;  
    int arg_index = 0;

    // Split the command into tokens
    char *token = strtok(cmd, " ");
    while (token != NULL && arg_index < ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            input_file = token;  
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            output_file = token; 
        } else {
            args[arg_index++] = token;  
        }
        token = strtok(NULL, " ");
    }
    args[arg_index] = NULL; 

    // Handle input redirection
    if (input_file != NULL) {
        input_fd = open(input_file, O_RDONLY);
        if (input_fd < 0) {
            perror("Input file opening failed");
            return;
        }
    }

    // Handle output redirection
    if (output_file != NULL) {
        output_fd = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (output_fd < 0) {
            perror("Output file opening failed");
            return;
        }
    }

    char path[50];
    sprintf(path, "/home/anurag/.bin/%s", args[0]);  
    args[0] = path;  // Update the first argument to the full path

    // Fork a process and execute the command
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid == 0) {
        // Child process executes the command
        execute_command(args, input_fd, output_fd);
    } else {
        // Parent process waits for the child to finish
        waitpid(pid, NULL, 0);
        if (input_fd != 0) close(input_fd);
        if (output_fd != 1) close(output_fd);
    }
}

// Function to process pipelines
void process_pipeline(char *cmd) {
    char *commands[ARGS];
    int index = 0;
    int pipe_fd[2], input_fd = 0;

    // Split the command by pipes
    char *token = strtok(cmd, "|");
    while (token != NULL && index < ARGS - 1) {
        commands[index++] = token;
        token = strtok(NULL, "|");
    }
    commands[index] = NULL;

    // Execute each command in the pipeline
    for (int i = 0; i < index; i++) {
        pipe(pipe_fd);  // Create a pipe for the current command
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            // Child process
            if (i > 0) {  // If not the first command, get input from the previous pipe
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (i < index - 1) {  // If not the last command, output to the pipe
                dup2(pipe_fd[1], STDOUT_FILENO);
            }
            close(pipe_fd[0]);  // Close read end of the pipe
            close(pipe_fd[1]);  // Close write end after duplication

            char *args[ARGS];
            int arg_index = 0;
            char *subtoken = strtok(commands[i], " ");
            while (subtoken != NULL && arg_index < ARGS - 1) {
                args[arg_index++] = subtoken;
                subtoken = strtok(NULL, " ");
            }
            args[arg_index] = NULL;

            // Modify args[0] for execv if necessary
            char path[50];
            sprintf(path, "/home/anurag/.bin/%s", args[0]);
            args[0] = path;

            execute_command(args, STDIN_FILENO, STDOUT_FILENO);
            exit(1);
        } else {
            // Parent process
            waitpid(pid, NULL, 0);
            close(pipe_fd[1]);  // Close the write end of the pipe
            input_fd = pipe_fd[0];  // Set up input for the next command
        }
    }
}

int main() {
    char input[COMMAND_LENGTH];

    while (1) {
        // Display prompt
        printf("anurag@anu $");
        fflush(stdout);

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets failed");
            return 1;
        }

        // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

        // Handle built-in commands
        if (strcmp(input, "exit") == 0) {
            break;  // Exit the shell
        } else if (strcmp(input, "help") == 0) {
            display_help();
        } else if (strcmp(input, "clear") == 0) {
            clear_screen();
        } else if (strchr(input, '|') != NULL) {
            // Handle pipelines
            process_pipeline(input);
        } else {
            // Handle single command with I/O redirection
            process_command(input);
        }
    }

    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <spawn.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <string.h>

// #define N 12

// extern char **environ;

// char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

// int isAllowed(const char*cmd) {
// 	// TODO
// 	// return 1 if cmd is one of the allowed commands
// 	// return 0 otherwise
	
// 	return 0;
// }

// int main() {

//     // TODO
//     // Add variables as needed

//     char line[256];

//     while (1) {

// 	fprintf(stderr,"rsh>");

// 	if (fgets(line,256,stdin)==NULL) continue;

// 	if (strcmp(line,"\n")==0) continue;

// 	line[strlen(line)-1]='\0';

// 	// TODO
// 	// Add code to spawn processes for the first 9 commands
// 	// And add code to execute cd, exit, help commands
// 	// Use the example provided in myspawn.c

//     }
//     return 0;
// }



#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12
#define MAX_ARGS 20

extern char **environ;

char *allowed[N] = {"cp", "touch", "mkdir", "ls", "pwd", "cat", "grep", "chmod", "diff", "cd", "exit", "help"};

int isAllowed(const char *cmd) {
    for (int i = 0; i < N; i++) {
        if (strcmp(cmd, allowed[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void execute_external_command(char *argv[]) {
    pid_t pid;
    int status;
    if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ) == 0) {
        waitpid(pid, &status, 0);
    } else {
        printf("NOT ALLOWED!\n");
    }
}

void print_help() {
    printf("The allowed commands are:\n");
    for (int i = 0; i < N; i++) {
        printf("%s\n", allowed[i]);
    }
}

int main() {
    char line[256];
    char *argv[MAX_ARGS + 1];

    while (1) {
        fprintf(stderr, "rsh> ");
        if (fgets(line, sizeof(line), stdin) == NULL) continue;

        line[strlen(line) - 1] = '\0';  // Remove newline

        if (strcmp(line, "") == 0) continue;

        // Tokenize input
        int argc = 0;
        char *token = strtok(line, " ");
        while (token != NULL && argc < MAX_ARGS) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;  // Null-terminate the array

        if (!isAllowed(argv[0])) {
            printf("NOT ALLOWED!\n");
            continue;
        }

        if (strcmp(argv[0], "exit") == 0) {
            return 0;
        } else if (strcmp(argv[0], "help") == 0) {
            print_help();
        } else if (strcmp(argv[0], "cd") == 0) {
            if (argc > 2) {
                printf("-rsh: cd: too many arguments\n");
            } else {
                if (chdir(argc == 2 ? argv[1] : getenv("HOME")) != 0) {
                    perror("rsh: cd");
                }
            }
        } else {
            execute_external_command(argv);
        }
    }
    return 0;
}

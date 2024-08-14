#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    char buf[512];
    int n;
    char *p = buf;
    char *args[MAXARG];
    
    // Initialize arguments, copying argv to args
    for (int i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }
    
    // Read from standard input one character at a time
    while ((n = read(0, p, 1)) > 0) {
        if (*p == '\n') {  // End of the line
            *p = 0;  // Null-terminate the string
            args[argc-1] = buf;  // Add the read line as the next argument
            args[argc] = 0;  // Null-terminate the argument list

            if (fork() == 0) {  // Child process
                exec(args[0], args);  // Execute the command
                exit(0);  // Shouldn't reach here if exec succeeds
            } else {  // Parent process
                wait(0);  // Wait for child to finish
            }

            p = buf;  // Reset the buffer pointer for the next line
        } else {
            p++;
        }
    }
    
    exit(0);
}

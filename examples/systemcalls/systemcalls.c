#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *   Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    // Call system() to execute the command
    // system() returns -1 on error, or the exit status of the command
    int result = system(cmd);
    
    // Check if system() call itself failed
    if (result == -1) {
        // system() failed to execute
        return false;
    }
    
    // Check if the command executed successfully (exit status 0)
    // WIFEXITED checks if child terminated normally
    // WEXITSTATUS gets the exit status
    if (WIFEXITED(result) && WEXITSTATUS(result) == 0) {
        return true;
    }
    
    // Command failed or returned non-zero exit status
    return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    // Fork a child process
    pid_t pid = fork();
    
    if (pid == -1) 
    {
        // Fork failed
        perror("fork");
        va_end(args);
        return false;
    }
    
    if (pid == 0) 
    {
        // Child process: execute the command
        // execv replaces the current process image with a new process
        // command[0] is the full path to the executable
        // command is the array of arguments (including command name as first element)
        execv(command[0], command);
        
        // If execv returns, it failed
        perror("execv");
        exit(EXIT_FAILURE);
    }
    
    // Parent process: wait for child to complete
    int status;

    if (waitpid(pid, &status, 0) == -1) 
    {
        perror("waitpid"); // waitpid failed
        va_end(args);
        return false;
    }
    
    va_end(args);
    
    // Check if child terminated normally and with exit status 0
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) return true;
    
    // Command failed or returned non-zero exit status
    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    // Fork a child process
    pid_t pid = fork();
    
    if (pid == -1) 
    {
        // Fork failed
        perror("fork");
        va_end(args);
        return false;
    }
    
    if (pid == 0) 
    {
        // Child process: redirect stdout and execute command
        
        // Open the output file for writing
        // O_WRONLY: write only, O_CREAT: create if doesn't exist, O_TRUNC: truncate to 0 length
        // 0644: file permissions (rw-r--r--)
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
        if (fd == -1) 
        {
            // Failed to open output file
            perror("open");
            exit(EXIT_FAILURE);
        }
        
        // Redirect stdout (file descriptor 1) to the output file
        // dup2 makes fd 1 (stdout) point to the same file as fd
        if (dup2(fd, STDOUT_FILENO) == -1) 
        {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }
        
        // Close the original file descriptor as we've duplicated it to stdout
        close(fd);
        
        // Execute the command (stdout is now redirected to the file)
        execv(command[0], command);
        
        // If execv returns, it failed
        perror("execv");
        exit(EXIT_FAILURE);
    }
    
    // Parent process: wait for child to complete
    int status;
    if (waitpid(pid, &status, 0) == -1) 
    {
        // waitpid failed
        perror("waitpid");
        va_end(args);
        return false;
    }
    
    va_end(args);
    
    // Check if child terminated normally and with exit status 0
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) return true;
    
    // Command failed or returned non-zero exit status
    return false;
}

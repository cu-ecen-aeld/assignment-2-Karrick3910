/*
 * writer.c - C application to write content to a file
 * Usage: writer <writefile> <writestr>
 * 
 * This application serves as an alternative to writer.sh script.
 * It does not create directories - assumes directory exists.
 * Uses syslog for logging with LOG_USER facility.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

/**
 * @brief Main function for the writer application
 * 
 * This function writes a string to a specified file. It uses syslog for logging
 * operations and errors. The application does not create directories - it assumes
 * the target directory already exists.
 * 
 * @param argc Number of command line arguments (should be 3: program name + 2 args)
 * @param argv Array of command line argument strings
 *             argv[1] - writefile: Path to the file to write to
 *             argv[2] - writestr: String content to write to the file
 * 
 * @return 0 on success, 1 on failure
 * 
 * @note Uses syslog with LOG_USER facility for logging
 * @note Logs write operations with LOG_DEBUG level
 * @note Logs errors with LOG_ERR level
 */
int main(int argc, char *argv[])
{

    /* ============================================================================
    *                       OPENLOG PARAMETERS
    * ============================================================================
    *    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);
    *            │         │                   │
    *            │         │                   └─── Facility (LOG_USER)
    *            │         └─────────────────────── Options (PID + Console)
    *            └───────────────────────────────── Identity ("writer")
    * 
    *    LOG_PID  : Include process ID in messages
    *    LOG_CONS : Write to console if syslog unavailable
    *    LOG_USER : Generic user-level messages facility
    *    "writer"    → Program identifier in log messages 
    * 
    * ============================================================================
    * */
    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);
    
    // Check if both arguments are provided
    if (argc != 3) {
        syslog(LOG_ERR, "Invalid number of arguments. Expected 2, got %d", argc - 1);
        fprintf(stderr, "Error: Two arguments required.\n");
        fprintf(stderr, "Usage: %s <writefile> <writestr>\n", argv[0]);
        closelog();
        return 1;
    }
    
    // Extract arguments
    const char *writefile = argv[1];
    const char *writestr = argv[2];

    /* ============================================================================
    *                         SYSLOG LEVELS USED
    * ============================================================================
    *    ┌───────────────────────────────────────────┐
    *    │  Level    │  Value  │  Description        │
    *    ├───────────┼─────────┼─────────────────────┤
    *    │ LOG_ERR   │    3    │ Error conditions    │
    *    │ LOG_DEBUG │    7    │ Debug messages      │
    *    └───────────┴─────────┴─────────────────────┘
    **/

    // Log the write operation
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    /**
     * ║   fopen(writefile, "w") behavior:                            
     * ║   Error Handling:                                             
     * ║   ✗ NULL returned → Log error + cleanup + exit               
     * ║   ✓ Valid FILE*  → Continue to write operation              
     **/

    FILE *file = fopen(writefile, "w");
    if (file == NULL) {
        syslog(LOG_ERR, "Failed to open file %s for writing: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Could not create file %s\n", writefile);
        closelog();
        return 1;
    }

    /**
     * ║   fprintf(file, "%s", writestr) behavior:
     * ║   ┌────────────────────────────────────────────────────────|
     * ║   │ • Writes formatted string to file stream               │ 
     * ║   │ • Returns number of characters written (≥0)            │ 
     * ║   │ • Returns negative value on error                      │ 
     * ║   │ • No automatic newline added                           │ 
     * ║   └────────────────────────────────────────────────────────| 
     * ║                                                               
     * ║   Data Flow:                                                  
     * ║   writestr → fprintf() → file buffer → disk                  
     * ║                                                               
     * ║   Error Cases: Disk full, permission denied, I/O error       
     **/

    if (fprintf(file, "%s", writestr) < 0) {
        syslog(LOG_ERR, "Failed to write to file %s: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Could not write to file %s\n", writefile);
        fclose(file);
        closelog();
        return 1;
    }
    
    // Close the file
    if (fclose(file) != 0) {
        syslog(LOG_ERR, "Failed to close file %s: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Could not close file %s\n", writefile);
        closelog();
        return 1;
    }
    
    // Close syslog
    closelog();
    
    return 0;
}
#include "lib.hh"

#if defined(__linux__)

#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <string.h>

bool write_hello_world(const char* file)
{
    int fd;
    ssize_t bytes_written;
    const char *data = "Hello, World!\n ";
    
    // Open (or create) a file for writing
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return false;
    }

    // Write data to the file
    bytes_written = write(fd, data, strlen(data));
    if (bytes_written == -1) {
        close(fd);
        return false;
    }

    // Close the file descriptor
    close(fd);
    return true;
}


#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

#include <windows.h>

bool write_hello_world(const char* file)
{
    HANDLE hFile;
    DWORD dwBytesWritten = 0;
    LPCSTR data = "Hello, World!\r\n";
    
    // Create a new file or open an existing one
    hFile = CreateFile(
        file,                      // File name
        GENERIC_WRITE,             // Open for writing
        0,                         // Do not share
        NULL,                      // Default security
        CREATE_ALWAYS,             // Overwrite existing file
        FILE_ATTRIBUTE_NORMAL,     // Normal file
        NULL                       // No template
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Write data to the file
    BOOL bErrorFlag = WriteFile(
        hFile,                   // Open file handle
        data,                    // Start of data to write
        strlen(data),            // Number of bytes to write
        &dwBytesWritten,         // Number of bytes that were written
        NULL                     // No overlapped structure
    );

    if (!bErrorFlag) {
        CloseHandle(hFile);
        return false;
    }

    // Close the file handle
    CloseHandle(hFile);
    return true;
}
#endif
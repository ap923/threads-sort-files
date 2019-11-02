# threads-sort-files
 C program that uses two processes (not threads) to sort the regular files in a directory and the sub-directories based on file sizes
The program spawns one process (i.e., child process) to scan the directory and the sub-directories
for regular files. When the (child) process finds a regular file, it obtains the size, and sends the size
and the pathname of the file to the parent process through a FIFO (named pipe). The parent process
sorts (in ascending order) and organizes the size and pathname information. When the child
process finishes scanning, it notifies the parent process, and the parent process prints out the sizes
and pathnames of the files. 

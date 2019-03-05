% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 59.65    0.158783          16     10003           write
 40.31    0.107309          11     10002           read
  0.02    0.000047          12         4           close
  0.01    0.000038          38         1         1 lseek
  0.00    0.000000           0         4           fstat
  0.00    0.000000           0         5           mmap
  0.00    0.000000           0         4           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         3         3 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         1           arch_prctl
  0.00    0.000000           0         4           openat
------ ----------- ----------- --------- --------- ----------------
100.00    0.266177                 20036         4 total

syscall-write: the system takes 10003 times to write from buffer to the destination file

syscall-read: the system takes 10002 times to copy bytes from source file to buffer

syscall-lseek: reposition read/write file offset.

syscall-close: the source file and destination file are close each time. SO the calls are 2

syscall-fstat: the system goes to find the file by pathname. So to open and to close each file counts as 2 calls. For both files, the total counts are 4.

syscall-mmap: system creates a mapping in the virtual address and map the files into memory. Considering the total times of open and close, there should be 4 calls. I just don't understand why it is 5

syscall-mprotect: system sets protection on a region of memory. The open and close proccess tries to access the memory so that there are 4 times of mprotect on the memory

syscall-brk: change data segment size. It defines the end of process's data segment.

syscall-access: it checks user's permission for a file. In this case, the 3 errors seem to happen because the system cannot find the file "nohwcap" and "preload" according to the record by using the command "strace ./fc"

syscall-execve: execute the program

syscall-arch_prctl:system sets a thread or process fot the program

syscall-openat: open a file or possibly create a file.

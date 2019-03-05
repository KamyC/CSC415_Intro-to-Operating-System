# CSC 415 - Project 2 - File copy

## Student Name : Jinghan Cao

## Student ID   : 918818659

## Build Instructions
### Use "make" to build the file  

## Run Instructions
### Use "./fc" to run  

## strace/dtruss output 
![image](https://github.com/csc415-01-fa18/csc415-p2-KamyC/blob/master/strace_output.png)
	
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
  

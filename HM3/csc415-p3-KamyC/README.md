# CSC 415 - Project 3 - My Shell

## Student Name: Jinghan Cao  

## Student ID: 918818659

## Build Instructions  
### make -> ./myshell  

## Run Instructions  
### ls -l
Jinghan-C >> ls -l  
total 220  
-rw-r--r-- 1 jinghan jinghan 178699 Oct 14 16:12 415_HW3.pdf  
-rw-r--r-- 1 jinghan jinghan    157 Oct 14 16:12 CMakeLists.txt  
-rw-r--r-- 1 jinghan jinghan   1074 Oct  8 18:48 LICENSE  
-rw-r--r-- 1 jinghan jinghan     82 Oct 14 16:12 Makefile  
-rwxr-xr-x 1 jinghan jinghan  13600 Oct 20 14:30 myshell  
-rw-r--r-- 1 jinghan jinghan   7527 Oct 20 14:28 myshell.c  
-rw-r--r-- 1 jinghan jinghan    246 Oct 19 23:49 README.md  
drwxr-xr-x 3 jinghan jinghan   4096 Oct 19 00:44 trial  
### cat LICENSE  
Jinghan-C >> cat LICENSE  
MIT License  
  
Copyright (c) 2018 SFSU-Repo-Storage  

Permission is hereby granted, free of charge, to any person obtaining a copy  
of this software and associated documentation files (the "Software"), to deal  
in the Software without restriction, including without limitation the rights  
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell  
copies of the Software, and to permit persons to whom the Software is  
furnished to do so, subject to the following conditions:  

The above copyright notice and this permission notice shall be included in all  
copies or substantial portions of the Software.  
    
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  
SOFTWARE.  
### ls -al /home
Jinghan-C >> ls -al /home  
total 12  
drwxr-xr-x  3 root    root    4096 Sep  6 16:43 .  
drwxr-xr-x 24 root    root    4096 Oct  1 20:37 ..  
drwxrwxrwx 32 jinghan jinghan 4096 Oct 20 14:16 jinghan  
### ls -l &
Jinghan-C >> ls -l &  
5200   
Jinghan-C >> total 220  
-rw-r--r-- 1 jinghan jinghan 178699 Oct 14 16:12 415_HW3.pdf  
-rw-r--r-- 1 jinghan jinghan    157 Oct 14 16:12 CMakeLists.txt  
-rw-r--r-- 1 jinghan jinghan   1074 Oct  8 18:48 LICENSE  
-rw-r--r-- 1 jinghan jinghan     82 Oct 14 16:12 Makefile  
-rwxr-xr-x 1 jinghan jinghan  13600 Oct 20 14:30 myshell  
-rw-r--r-- 1 jinghan jinghan   7527 Oct 20 14:28 myshell.c  
-rw-r--r-- 1 jinghan jinghan    246 Oct 19 23:49 README.md  
drwxr-xr-x 3 jinghan jinghan   4096 Oct 19 00:44 trial  
### ls -al /home &
Jinghan-C >> ls -al /home &  
478  
Jinghan-C >> total 12  
drwxr-xr-x  3 root    root    4096 Sep  6 16:43 .  
drwxr-xr-x 24 root    root    4096 Oct  1 20:37 ..  
drwxrwxrwx 32 jinghan jinghan 4096 Oct 20 18:42 jinghan  
### ls -l > outfile
### ls -l >> outfile
### ls -al /home > outfile2
### ls -al /home >> outfile2
### grep permission < LICENSE
Jinghan-C >> grep permission < LICENSE  
The above copyright notice and this permission notice shall be included in all  
### grep SOFTWARE < LICENSE
Jinghan-C >> grep SOFTWARE < LICENSE  
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  
SOFTWARE.  
### ls -al /home | grep root
Jinghan-C >> ls -al /home | grep root  
drwxr-xr-x  3 root    root    4096 Sep  6 16:43 .  
drwxr-xr-x 24 root    root    4096 Oct  1 20:37 ..  
### cd 1/2/3
### pwd
Jinghan-C >> cd 1/2/3   
Jinghan-C >> pwd   
/home/jinghan/Documents/CSC415/HM3/csc415-p3-KamyC/1/2/3   
### exit

## List Extra Credits comepleted (if non attempted leave blank)

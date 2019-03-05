# CSC 415 - Project 4 - Thread Racing

## Student Name: Jinghan Cao

## Student ID :  918818659

## Build Instructions
### Make

## Run Instructions
### ./threadracer

## Explain why your program produces the wrong output
### Nanosleep interferes the execution of functions.    
For example, if I add nanosleep in the subtractor function, most of the subtractor functions are executed after the adder functions due to the nanosleep which makes the subtraction delayed. To be more specific, in the main function there are 16 threads are created and the even numbered are designated for the adder function. When the for-loop interates, the 0,2,4,6... threads are created to add the global value. And 1,3,5...threads which are for subtraction are still asleep.  

One scenario is that, when the for-loop iterates to 4,6,8 or some even number, the threads numbered 1 or 3 are awake and they started to execute the subtraction. However when all the adder threads are finished and they are joined via pthread_join. There may be several threads for subtraction are still alseep even when the printf function to display the final global value. This is likely to result in the output to be other numbers other than zero.


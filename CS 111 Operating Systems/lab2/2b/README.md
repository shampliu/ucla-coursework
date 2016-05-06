## CS 111 Project 2A - Atomic Operations 
### Chang Liu

#### Included Files:
* SortedList.h - the header file for the SortedList implementation
* SortedList.c - SortedList implementation, implementing insert, delete, lookup and length functionality
* lab2b.c - source code for the main program, allows for 3 possible lock types (unprotected, spin, mutex), yield flags for insert, delete and search operations, and a user-inputted amount of threads and iterations
* iterations-vs-time.png - graph showing the average cost / operation on a single unprotected thread as the iterations increase
* threads-vs-time.png - graph showing the corrected average cost / operation (per element) with 1000 iterations on each type of lock as we vary the number of threads

#### Questions:
1A. The behavior of the graph is similar to what we saw in Lab 2A. Initially, there is setup overhead from creating and stopping a thread so the average cost / operation is high. As we increase the iterations, the setup costs are amortized since it is fixed on a single thread so the average cost / op lowers. However, at a certain point, the number of iterations cause the run time of the operations to be quite costly (exceeding the costs of creating / joining threads) and so different from Lab 2A's program, this program becomes more costly after this point. 

1B. We can correct this effect by dividing the average cost per op by the total number of elements in the list. 

2A. The difference in graphs can be explained in several points. First, the critical section of SortedList is much longer than the one in the add() function of Lab 2A so the lock is potentially held for a longer time since we have to search a longer data structure than the 2A one. Secondly, since the threads hold the lock longer now, the probability of conflict is much higher for the same number of threads and more conflicts means more blocked threads, more overhead and less parallelism. 
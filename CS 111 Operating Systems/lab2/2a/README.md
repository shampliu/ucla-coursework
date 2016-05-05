CS 111 Project 2A - Atomic Operations
Chang Liu

Included Files:
* lab2a.c - the source code implementing 4 possible thread functions - using no lock, pthread_mutex, spin lock, or compare and swap
* Makefile - contains targets for building, cleaning, and distributing the project
* threads-vs-time.png - graph showing the performance of all 4 versions with 10000 iterations on a varying number of threads
* iterations-vs-time.png - graph showing the performance of a single thread using no lock with a varying number of iterations

Questions:

Running lab2a without locks on a single thread doesn't generate any failures, regardless of the iteration amount. When I go to 2 threads, I need about 5,000 iterations to consistently generate an error. As I bump up the number of threads, it takes less iterations to fail. 

1A. Why does it take this many threads or iterations to result in failure?
The critical operation, which is the add is a very short function. Thus to increase the probability that two or more threads are in a race condition trying to modify the same counter, we must increase the threads and / or iterations. 

1B. Why does a significantly smaller number of iterations so seldom fail?
Less iterations means less chance for the rare race condition to occur. 

With the yield option on, it only takes around 200 iterations to generate a failure on 2 threads (and thus even less on more threads). 

2A. Why does the average cost per operation drop with increasing iterations?
The cost to create each thread is much larger than the cost of the operations. AVG COST = (THREAD CREATION + RUNTIME) / OPS. However, thread creation time is nearly constant so when we increase the iterations, thereby increasing the number of operations in the denominator, we lower the average cost. 

2B. How do we know what the "correct" cost is?
We can get a good sense when we have a high number of iterations to account for the thread creation overhead. 

2C. Why are the --yield runs so much slower? Where is the extra time going?
The pthread_yield function causes the current thread to relinquish the CPU. We call this twice per iteration (one for add, one for subtract) if the yield flag is set so it gets called 2N iterations. Relinquishing the CPU for a thread means context switching to another, requiring the saving and loading of state which could be costly if it occurs 2N times. 

2D. Can we get valid timings if we are using --yield? How, or why not?
Yes, we would have to know how much time a context switch takes to happen. We can do this by removing the operations in our code and just running it with yield.  

3A. Why do all the options perform similarly for low numbers of threads?
Lower amount of threads means less chance for conflict / race conditions, which means less use for locking. The time spent spinning / waiting for a lock will be negligible if there are not many threads in contention for that lock. Upping the number of threads will show clear differences in the 4 different methods. 

3B. Why do the three protected operations slow down as the number of threads rise?
With more contention for a lock, the locking operations are invoked more and thus creates a lot of overhead from just locking / unlocking. The overhead significantly slows down the protected operations.

3C. Why are spin-locks so expensive for large number of threads?
The spinning part is the expensive part. When a lock is held by another thread, the current CPU will just spin and do nothing until the lock is released. 


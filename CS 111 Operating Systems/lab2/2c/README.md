## CS 111 Project 2C - Lock Granularity and Performance
### Chang Liu

#### Included Files:
* SortedList.h - the header file for the SortedList implementation
* SortedList.c - SortedList implementation, implementing insert, delete, lookup and length functionality
* lab2c.c - source code for the main program, allows for 3 possible lock types (unprotected, spin, mutex), yield flags for insert, delete and search operations, and a user-inputted amount of threads, iterations, and sublists
* thread-to-list-vs-cost.png - graph showing the average cost / operation / sublist on the 3 different synchronized methods as the ratio of threads-to-list increases

#### Questions:
1A. With the synchronized methods, I ran them with 8 threads, instead of 1, like how I did with unprotected. In the beginning, the number of lists is significantly larger than the amount of threads so contention for the the same list is less likely. For that reason, we see a performance boost in the mutex and spin-lock options compared to the unprotected method. However, as the ratio of threads to lists increases, the performance inversely decreases as we notice more contention on the lists since we have fewer lists.

1B. Since we implemented sublists in this Lab, the ratio of threads to lists is more interesting than just threads. We have to take into account the number of lists now. If we just analyzed the number of threads, we have less context and less idea of how much contention is going on in the program. For example, in 1A we notice that performance significantly slows when threads per list increase. In addition, it would be less accurate if we only counted the threads because we could have multiple threads per list and multiple lists per thread. 

2A. With a fixed amount of threads, increasing the number of lists has an inverse relationship with the performance. When the lists are greater than the number of threads, the performance improves since as we saw previously, there is less contention for the lists.  

2B. In every case except for the case of a single list, spin-lock does better than mutex. This could be because of spin-locks large memory requirements when multiple threads are spinning and especially when the contended item (1 list) is sparse. As we increase lists, the amount of mutex locks increase so there is more overhead from initializing, locking, and unlocking those locks while spin-locks do not share the same overhead. 

3A. If we don't hold the mutex, other threads can change the condition variable. We also don't know that so we may sleep forever in that case. 

3B. If we don't release the mutex, then other threads cannot change the condition variable. We then reach a deadlock because the waiting thread is blocked while holding the lock but the child thread will sleep forever because it is waiting for a lock that will never be released. 

3C. The mutex should be reacquired when the calling function resumes to ensure there is exclusive access to the done variable, or else other threads can change it. There is a while loop that spins to wait for a change in the done variable so once this changes, there should be only one thread to access it. 

3D. If the lock is released before sleeping, the current thread may never wake up. 

3E. We can't implement this in user mode because we need preemption, which is not available with user privileges. In most systems, it is not possible to prevent any user mode computation from being preempted. 

#### Performance Analysis:
./lab2c --lists=1 --iterations=5000
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
100.18      0.02     0.02     5000     4.01     4.01  SortedList_insert
  0.00      0.02     0.00    10000     0.00     0.00  hash
  0.00      0.02     0.00     5000     0.00     0.00  SortedList_delete
  0.00      0.02     0.00     5000     0.00     0.00  SortedList_lookup
  0.00      0.02     0.00        2     0.00     0.00  SortedList_length

./lab2c --lists=1 --iterations=10000
-----------------------------------------------------------------------
 %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 53.94      0.07     0.07    10000     7.01     7.01  SortedList_lookup
 46.24      0.13     0.06    10000     6.01     6.01  SortedList_insert
  0.00      0.13     0.00    20000     0.00     0.00  hash
  0.00      0.13     0.00    10000     0.00     0.00  SortedList_delete
  0.00      0.13     0.00        2     0.00     0.00  SortedList_length

./lab2c --lists=1 --iterations=20000
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 57.80      0.30     0.30    20000    15.03    15.03  SortedList_insert
 42.39      0.52     0.22    20000    11.02    11.02  SortedList_lookup
  0.00      0.52     0.00    40000     0.00     0.00  hash
  0.00      0.52     0.00    20000     0.00     0.00  SortedList_delete
  0.00      0.52     0.00        2     0.00     0.00  SortedList_length

With no locks, the time spent in various functions is distributed similarly as we have a higher number of iterations. However, for 5000 iterations, the most time-consuming function of the insert; I'm assuming this is because the list does not have a significant amount of elements to need to lookup quickly. The number of calls is consistent here.

./lab2c --lists=1 --iterations=10000 --threads=4 --sync=m
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 56.73      0.94     0.94    39986    23.55    23.55  SortedList_insert
 43.45      1.66     0.72    39999    18.03    18.03  SortedList_lookup
  0.00      1.66     0.00    79970     0.00     0.00  hash
  0.00      1.66     0.00    40000     0.00     0.00  SortedList_delete
  0.00      1.66     0.00        5     0.00     0.00  SortedList_length

./lab2c --lists=4 --iterations=10000 --threads=4 --sync=m
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 55.00      0.28     0.28    39435     7.11     7.11  SortedList_insert
 43.22      0.50     0.22    39339     5.60     5.60  SortedList_lookup
  1.96      0.51     0.01       20   500.91   500.91  SortedList_length
  0.00      0.51     0.00    78582     0.00     0.00  hash
  0.00      0.51     0.00    39357     0.00     0.00  SortedList_delete

./lab2c --lists=8 --iterations=10000 --threads=4 --sync=m
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 57.80      0.15     0.15    38819     3.87     3.87  SortedList_insert
 34.68      0.24     0.09    38345     2.35     2.35  SortedList_lookup
  3.85      0.25     0.01       40   250.46   250.46  SortedList_length
  3.85      0.26     0.01                             thread_func
  0.00      0.26     0.00    76676     0.00     0.00  hash
  0.00      0.26     0.00    38469     0.00     0.00  SortedList_delete

With mutex locks implemented, the distribution among the functions are similar. As we increase the lists, we see a gradual focus on the insert function, and less time spent in lookup. Also, the total calls do not add up, and they become less with more lists. We also see that the thread_func starts playing a minor role in the time when we hit 8 lists.

./lab2c --lists=1 --iterations=10000 --threads=4 --sync=s
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 97.71     55.44    55.44                             thread_func
  1.26     56.16     0.72    39546    18.11    18.11  SortedList_insert
  1.20     56.84     0.68    39775    17.13    17.13  SortedList_lookup
  0.01     56.84     0.01                             frame_dummy
  0.00     56.84     0.00    79757     0.00     0.00  hash
  0.00     56.84     0.00    39999     0.00     0.00  SortedList_delete
  0.00     56.84     0.00        5     0.00     0.00  SortedList_length

./lab2c --lists=4 --iterations=10000 --threads=4 --sync=s
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 73.49      1.90     1.90                             thread_func
 16.63      2.33     0.43    32803    13.13    13.13  SortedList_insert
  9.67      2.58     0.25    35039     7.15     7.15  SortedList_lookup
  0.39      2.59     0.01    78384     0.13     0.13  hash
  0.00      2.59     0.00    39372     0.00     0.00  SortedList_delete
  0.00      2.59     0.00       18     0.00     0.00  SortedList_length

./lab2c --lists=8 --iterations=10000 --threads=4 --sync=s
-----------------------------------------------------------------------
  %   cumulative   self              self     total
 time   seconds   seconds    calls  us/call  us/call  name
 60.11      0.45     0.45                             thread_func
 20.04      0.60     0.15    32642     4.60     4.60  SortedList_lookup
 20.04      0.75     0.15    31003     4.85     4.85  SortedList_insert
  0.00      0.75     0.00    76366     0.00     0.00  hash
  0.00      0.75     0.00    38392     0.00     0.00  SortedList_delete
  0.00      0.75     0.00       37     0.00     0.00  SortedList_length

In the spin-lock method, we see more drastic shifts in the time spent on each function as we increase the lists. The biggest difference is that the thread_func takes up the most time consistently. As we increase the lists, less time is spent in the thread function and is distributed to mainly lookup and insert operations.
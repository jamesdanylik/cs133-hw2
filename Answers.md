Answers to Written Exercises
============================

Part One: A parrallel algorithm for calculating prefix sums.
------------------------------------------------------------

In order to calculate prefix sums, we can use a shared resource to store
the prefix sums calculated so far.

For a given list of input N units long, there are N prefix sums.  We create
a shared array to hold each prefix sum, and initialize them to an invalid
value. (linear, N ops)

Our group of threads is assigned a partion of the N prefix sums by modulo
the number of threads, giving each thread N/#Threads prefix sums to calculate,
evenly distributed.  Each thread proceeds to caclulate their assigned prefix
sums as follows:
	Check the array for entries lower then the current prefix sum. Use the
	greatest seen entry as a starting point for the caclulation.  This entry`s
	prefix sum is a[x] + i[x+1] ... + i[n], where the a array is the prefix sum
	array and i holds input.  On avearge, each thread will have to do one pass
	thru the array (linear, n ops) and at worst two array access + two addition.
	It then stores the result in the appropraite place in the prefix sum array.

This proceeds until all prefix sums are caclulated. 

Part Two: Questions Relating Parrallel is_prime().
--------------------------------------------------

*	1) The results from running my code on the SEASNET LnxSrv follow. I
	redirected the normal output of the functions for cleanliness.
	# N = 20 Million, T = 1
	$ time ./hw2a 20000000 1 > /dev/null
	> real 0m23.591s
	> user 0m23.516s
	> sys  0m0.013s
	# N = 20 Million, T = 2
	$ time ./hw2a 20000000 2 > /dev/null
	> real 0m15.838s
	> user 0m24.367s
	> sys  0m0.009s
	# N = 20 Million, T = 4
	$ time ./hw2a 20000000 4 > /dev/null
	> real 0m9.478s
	> user 0m24.870s
	> sys  0m0.010s
	# N = 20 Million, T = 8
	$ time ./hw2a 20000000 8 > /dev/null
	> real 0m6.706s
	> user 0m27.996s
	> sys  0m0.013s
	# N = 20 Million, T = 16
	$ time ./hw2a 20000000 16 > /dev/null
	> real 0m5.352s
	> user 0m39.660s
	> sys  0m0.008s

*	2) The results from part B, where dynamic scheduling is enabled and set to a
	chunk size of 1:
	# N = 20 Million, T = 1, CS = 1
	> real 0m23.683s
	> user 0m23.612s
	> sys  0m0.016s
	# N = 20 Million, T = 2, CS = 1
	> real 0m13.256s
	> user 0m24.760s
	> sys  0m0.003s
	# N = 20 Million, T = 4, CS = 1
	> real 0m7.590s
	> user 0m25.222s
	> sys  0m0.007s
	# N = 20 Million, T = 8, CS = 1
	> real 0m5.531s
	> user 32.783s
	> sys  0m0.007s
	# N = 20 Million, T = 16, CS = 1
	> real 0m4.676s
	> user 0m42.818s
	> sys  0m0.011s
	
	Looking at our results, dynamic scheduling offers a substantial improvement
	over static scheduling.  Because the order of the prime-caclulation threads
	does not matter, we can obtain additional performance by not restricting the
	threads to be created in any particular order either.

	These are the results for 16 threads, varying chunksize:
	# T = 16, CS = 1 ( above )
	# T = 16, CS = 2
	> real 0m4.689s
	> user0m42.718s
	> sys  0m0.015s
	# T = 16, CS = 4
	> real 0m4.676s
	> user 0m42.286s
	> sys  0m0.007s
	# T = 16, CS = 8
	> real 0m4.641s
	> user 0m41.857s
	> sys  0m0.013s
	# T = 16, CS = 16
	> real 0m4.690s
	> user 0m41.794s
	> sys 0m0,017s
	# T = 16, CS = 32
	> real 0m4.674s
	> user 0m42.152s
	> sys  0m0.015s

	Yes, but the returns are limited and diminish quickly.  Scheduling in dynamic
	chunks up to the thread count makes sense, and can keep input ready as the
	threads do their work.  But scheduling more work then threads make little
	sense as it occurs additional overhead preparing work the threads will not be
	able to do for some time.

*	3) When two threads are accessing the shared queue, there are two main
	situations that can cause problems.  First, the buffer may be in use by
	another thread, and updating it could cause collisons and unexpected
	behavior.  Second, the threads may compete over the buffer, with one
	always winning; this could result in a deadlock.  

	I resolved these problems by using an OpenMP lock to protect the buffer.
	Before using the resource, each thread must attain the lock and return
	it immidiately after performing its buffer operation.  I`ve also ensured
	that the consumer wins in compitions for this resource and idles when the
	buffer as empty ( and similarly for the producer ) to avoid deadlock 
	situations.

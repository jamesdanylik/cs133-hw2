#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <omp.h>

// The is prime function as give in the assignment, slightly reordered to
// avoid allocating 'i' unless neccasary.
int is_prime( int p ) {
	if ( p < 2 ) return 0;
	int i = 2;
	while ( i*i <= p ) {
		if ( p % i == 0 ) return 0;
		i++;
	}
	return 1;
}

int main( int argc, char* argv[] ) {
	// Check we have the right arguments and read them in.
	if ( argc != 3 ) { printf("Usage: ./hw2a MAX_NUM NUM_THREADS"); return 1; }
	int N = atoi( argv[1] );
	int T = atoi( argv[2] );

	// Set number of threads according to argument.
	omp_set_dynamic(0);
	omp_set_num_threads(T);

	// Create a priority queue to hold the thread's output in a sensible manner.
	std::priority_queue<int, std::vector<int>, std::greater<int> > pq;

	// Deal with 2 as a special case, to simplify and speed up the loop.
	int i;
	#pragma omp parallel private(i) shared(pq)
	{
		#pragma omp for schedule(dynamic,16) nowait
		for ( i = 3; i <= N; i += 2 ) {
			bool prime = is_prime(i);
			if ( prime ) {
				#pragma omp critical
				{
					pq.push(i);
				}
			}
		}	
	}

	bool output = ( N >= 2 );
	if ( output ) printf("2");
	while ( !pq.empty() ) {
		printf( ", %d", pq.top() );
		pq.pop();
	}
	if ( output ) printf("\n");
	return 0;
}

#include <cstdlib>
#include <cstdio>
#include <omp.h>
#include <math.h>

struct point_t { double x, y; };

#define CB_SIZE 32
class circle_buffer {
	point_t arr[CB_SIZE];
	int head;
	int size;
public:
	circle_buffer();
	bool empty();
	bool full();
	void put(point_t p);
	point_t get();
};

circle_buffer::circle_buffer() { head = 0; size = 0; }

bool circle_buffer::empty() { return (size == 0) ? true : false; }

bool circle_buffer::full() { return (size >= CB_SIZE-1) ? true : false; }

void circle_buffer::put(point_t p) {
	arr[head++] = p;
	if ( head >= CB_SIZE ) head = 0;
	size++;
}

point_t circle_buffer::get() {
	head--;
	if ( head < 0 ) head = CB_SIZE-1;
	point_t p = arr[head];
	size--;
	return p;
}

int main( int argc, char* argv[] ) {
	if ( argc != 2 ) { printf("Usage: ./hw2b #POINTS\n"); return 1; }
	int numPoints = atoi( argv[1] );

	int num_in = 0;
	circle_buffer cb;
	omp_lock_t cb_lock;
	omp_init_lock( &cb_lock );

	#pragma omp parallel sections shared(num_in,cb,cb_lock)
	{
		#pragma omp section
		{// The producer
			int i = 0;
			while ( i <= numPoints ) {
				point_t p;
				p.x = (double) rand()/RAND_MAX;
				p.y = (double) rand()/RAND_MAX;
				omp_set_lock( &cb_lock );
				if ( cb.full() ) {
					omp_unset_lock( &cb_lock );
					continue;
				}
				cb.put(p);
				omp_unset_lock( &cb_lock );
				i++;
			}
		}
		
		#pragma omp section
		{// The Consumer
			int i = 0;
			while ( i <= numPoints ) {
				point_t p;
				omp_set_lock( &cb_lock );
				if ( cb.empty() ) {
					omp_unset_lock( &cb_lock );
					continue;
				}
				p = cb.get();
				omp_unset_lock( &cb_lock );
				double internal = ( pow((p.x-0.5),2) + pow((p.y-0.5),2) );
				double r = sqrt(internal);
				if ( r <= 0.5 ) num_in++;
				i++;
			}
		}
	}
	double pi = 4.0 * ((double) num_in)/numPoints;
	printf("%.7f\n", pi);
	return 0;
}

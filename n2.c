// Generate Hamming intervals (intervals close to 1.0) for fractions having
// only 2 in the numerator, i.e., 2^i/(3^j*5^k) where i > 0 and j,k >= 0.
//
// The intervals start from 2.0 and work downward toward 1.0.
//
// usage ./n2 <max power of 10>
// where the max power of 10 of the Hamming numbers is given
//
// Joe Knapp  knapp.167@osu.edu
//
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// high-precision logs
// long doubles used (sizeof(long double) = 16 bytes on ubuntu)
#define LN2 0.693147180559945309417232121458L
#define LN3 1.09861228866810969139524523692L
#define LN5 1.60943791243410037460075933323L

int main(int argc, char **argv) {
	int i ;  // power of 2
	int j ;  // power of 3
	int k ;  // power of 5
	int p2max ;  // max power of 2
    int	p3max ;  // max power of 3
	long double L2, L3, L5, L2_3, L2_5, L3_5 ;  // precomputed logs
	long double f,logf, minlogf ; // f = interval

	if (argc != 2) {
        fprintf(stderr,"usage: %s <max power 10>\n",argv[0]) ;
        exit(1) ;
    }

	// set up precomputed logs
	L2 = LN2 ;
	L3 = LN3 ;
	L5 = LN5 ;
	L2_3 = L2/L3 ;
	L2_5 = L2/L5 ;
	L3_5 = L3/L5 ;

	// max value for i (2^i in numerator)
	p2max = ceil(atol(argv[1])*log(10.0)/log(2.0))  ;

	minlogf = 1000 ;  // minimum value for f seen so far, initialized to high value

	// loop through increasing powers of 2 in the numerator, finding the 
	// minimum interval over (j,k) for each case, and printing if it's a new minimum.
	for (i = 1 ; i <= p2max ; i++) {
		p3max = floor(L2_3*i) ;  // max value for j (3^j in denominator) 
		// loop backwards through j
	    for (j = p3max ; j >= 0 ; j--) {
		    k = floor(L2_5*i - L3_5*j) ; // k (5^k in denominator) directly computed
			logf = L2*i - L3*j - L5*k ;  // compute log of this interval for i,j,k

			// if new minimum, print it
			if (logf < minlogf) {
				printf("%d %d %d %.20Lf\n",i,-j,-k,(long double)exp(logf)) ;
				minlogf = logf ;
			}
		}
	}
}

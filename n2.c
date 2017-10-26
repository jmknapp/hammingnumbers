#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define LN2 0.693147180559945309417232121458L
#define LN3 1.09861228866810969139524523692L
#define LN5 1.60943791243410037460075933323L

int main(int argc, char **argv) {
	int p2max, p3max ;
	int i, j, k ;
	long double f,logf, minlogf, L2, L3, L5, L2_3, L2_5, L3_5 ;
	int ihtab ;

	if (argc != 2) {
        fprintf(stderr,"usage: %s <max power 10>\n",argv[0]) ;
        exit(1) ;
    }

	L2 = LN2 ;
	L3 = LN3 ;
	L5 = LN5 ;
	L2_3 = L2/L3 ;
	L2_5 = L2/L5 ;
	L3_5 = L3/L5 ;

	p2max = ceil(atol(argv[1])*log(10.0)/log(2.0))  ;

	minlogf = 1000 ;
	ihtab = 1 ;
	for (i = 1 ; i <= p2max ; i++) {
		p3max = floor(L2_3*i) ;
	    for (j = p3max ; j >= 0 ; j--) {
		    k = floor(L2_5*i - L3_5*j) ;
			logf = L2*i - L3*j - L5*k ;

			if (logf < minlogf) {
				printf("%d %d %d %.20Lf\n",i,-j,-k,(long double)exp(logf)) ;
				ihtab++ ;
				minlogf = logf ;
			}
		}
	}
}

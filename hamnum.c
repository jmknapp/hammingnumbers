// Generate nth Hamming number, for n < 1.5e12
// Joe Knapp  knapp.167@osu.edu
//
// To generate "band" of triples in the vicinity of the nth number,
// uses basic scheme of Louis Klauder described in:
// http://www.drdobbs.com/architecture-and-design/hamming-problem/228700538
// 
// Once the lower triple in the band is identified, and the offset
// from that number to the nth number determined, the table in
// hamnum.h is used to generate iterates up to the nth number.
//
#define TRUE 1
#define FALSE 0

#define LOG2 log(2.0)
#define LOG3 log(3.0)
#define LOG5 log(5.0)
#define THIRD (1.0/3.0)

#define LOWOFFSET 1.703
#define HIOFFSET 1.693

#define NMIN 50000   // below NMIN, just get the Nth number by chugging
                     // through the whole sequence

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hamnum.h"

void hammahead(unsigned long n, int *i0, int *j0, int *k0)  ;

int main(int argc, char **argv) {
    double l2 = LOG2, l3 = LOG3, l5 = LOG5 ; // precomputed logarithms
    double logM, logMlo, logMhi ; // per the Dr. Dobbs article, 
                                  // logMlo < i*l2 + j*l3 + k*l5 < logMhi
    int i, j, k ;  //loop counters 
    int imin, imax, jmax, kmax ;  // limits for i,j,k loops
    double logMmin = 1e9 ;  // minimum logM seen, defaults to big number

    unsigned long n ;  // want nth Hamming number
    unsigned long totn = 0 ;  // total i,j,k combinations below logMlo

    int i0, j0, k0 ; // i,j,k values for minimum logM in band
    unsigned long nahead ;  // number of values to skip ahead from i0, j0, k0

    unsigned char found = FALSE ;

    if (argc != 2) {
        fprintf(stderr,"usage: %s <n>\n",argv[0]) ;
        exit(1) ;
    }
    n = atol(argv[1]) ;

    if (n >= NMIN) {
        // get range of logM
        logM = pow((6.0*l2*l3*l5)*n,THIRD) ;
        logMlo = logM - LOWOFFSET ;
        logMhi = logM - HIOFFSET ;
    
        kmax = floor(logMhi/l5) ;
        for (k = 0 ; k <= kmax ; k++) {
            jmax = floor((logMhi - k*l5)/l3) ;
            for (j = 0 ; j <= jmax ; j++) {
                imin = ceil((logMlo - k*l5 - j*l3)/l2) ;
                imax = floor((logMhi - k*l5 - j*l3)/l2) ;
                totn += imin ;
                for (i = imin ; i <= imax ; i++) {
                    logM = i*l2 + j*l3 + k*l5 ;
                    if (logM < logMmin) {
                        logMmin = logM ;
                        i0 = i ;
                        j0 = j ;
                        k0 = k ;
                    }
                }
            }
        }
        nahead = n - totn - 1 ;
        hammahead(nahead,&i0,&j0,&k0) ;
    }
    else {
        int m, in ;
        i0 = 0 ;
        j0 = 0 ;
        k0 = 0 ;
        for (in = 0 ; in < n-1 ; in++) {
            m = 0 ;
            found = FALSE ;
            while (!found && m < NTAB) {
                if (i0 + hamtab[m].p2 >= 0)
                    if (j0 + hamtab[m].p3 >= 0)
                        if (k0 + hamtab[m].p5 >= 0)
                            found = TRUE ;
                m++ ;
            }
            m-- ;
            i0 += hamtab[m].p2 ;
            j0 += hamtab[m].p3 ;
            k0 += hamtab[m].p5 ;
        }
    }
    printf("%d %d %d\n",i0,j0,k0) ;  // output nth i,j,k values
}

// From the Hamming number 2^i0*3^j0*5^k0, skip ahead
// nhead numbers and update i0, j0 and k0 accordingly.
void hammahead(unsigned long nahead, int *i0, int *j0, int *k0) {
    unsigned char k ;
    unsigned long j ;
    unsigned char found ;

    for (j = 0 ; j < nahead ; j++) {
        // for each Hamming number, find the minimum interval in hamtab
        // i.e., the minimum entry where multiplication would result
        // in positive powers for i, j & k
        k = 0 ;
        found = FALSE ;
        while (!found && k < NTAB) {
            if (*i0 + hamtab[k].p2 >= 0)
                if (*j0 + hamtab[k].p3 >= 0)
                    if (*k0 + hamtab[k].p5 >= 0)
                            found = TRUE ;
            if (!found)
                k++ ;
        }

        if (!found) {
            fprintf(stderr,"error: suitable table entry not found, j=%ld\n",j) ;
            exit(1) ;
        }
        else {
            // got the next interval, update counts
            *i0 += hamtab[k].p2 ;
            *j0 += hamtab[k].p3 ;
            *k0 += hamtab[k].p5 ;
        }
    }
}

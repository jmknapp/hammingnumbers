// Print first n Hamming numbers starting from 1
//
// Joe Knapp knapp.167@osu.edu
//
#define TRUE 1
#define FALSE 0

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "hamseq.h"

// struct to hold generated list of Hamming numbers
struct {
	short i ;  // power of 2
	short j ;  // power of 3
	short k ;  // power of 5
} *hnums ;

int main(int argc, char **argv) {
    short p2, p3, p5 ;  // powers of 2, 3, and 5

    unsigned char ihtab ;  // into into Hamming interval table hamtab[]
    unsigned char istab ;  // index into successor table
    unsigned char inext ;  // index into successor list

    unsigned long ihn ;  // loop counter for Hamming numbers
    unsigned long n ;    // number of Hamming numbers desired

    unsigned char found = FALSE ;
    unsigned char savenums = TRUE ;  // TRUE to save generated values in hnums

	// check command line
    if (argc != 2) {
        fprintf(stderr,"usage: %s <length of list>\n",argv[0]) ;
        exit(1) ;
    }
    n = atol(argv[1]) ;

	// allocate memory for hnums struct
	// If can't allocate, set flag to turn off saving of intermediate
	// Hamming numbers, and continue.
	hnums = malloc(n*sizeof(hnums)) ;
	if (hnums == NULL) {
		fprintf(stderr,"error: can't allocate enough memory for list of size %ld (%ld bytes)\n",n,n*sizeof(hnums)) ;
		fprintf(stderr,"will compute, but not save list\n") ;
		savenums = FALSE ;
	}

    // first smooth number = 1 (2^0 * 3^0 * 5^0)
    p2 = 0 ;
    p3 = 0 ;
    p5 = 0 ;
    ihtab = NTAB-1 ;  // last entry in hamtab[] is interval 2

	// for each Hamming number
    for (ihn = 1 ; ihn < n ; ihn++) {
		// get next table entry
        inext = 0 ;
        found = FALSE ;
        while (!found && (inext < succtab[ihtab].n)) {
            istab = succtab[ihtab].i[inext] ;
            if (p2 + ftab[istab].p2 >= 0)
                if (p3 + ftab[istab].p3 >= 0)
                    if (p5 + ftab[istab].p5 >= 0)
                        found = TRUE ;
            inext++ ;
        }

        if (!found) {
            fprintf(stderr,"error: suitable table entry not found, ihn=%ld\n",ihn) ;
            exit(1) ;
        }
        else {
            ihtab = istab ;

            // update counts
            p2 += ftab[ihtab].p2 ;
            p3 += ftab[ihtab].p3 ;
            p5 += ftab[ihtab].p5 ;
			if (savenums) {
	            hnums[ihn].i = p2 ;
			    hnums[ihn].j = p3 ;
			    hnums[ihn].k = p5 ;
			}
        }
    }
    ihn-- ;
	printf("done, last Hamming number %d %d %d, n=%ld\n",p2,p3,p5,n) ;

	free(hnums) ;
}

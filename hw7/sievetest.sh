#!/bin/bash

TIME="/usr/bin/time -p"
OUT=../sievetest.`date +%m%d_%H:%M:%S`
for DIR in $*
do  cd $DIR
    for MAX in 1000000 100000000
    do  for NTHREADS in 1 4 16 32
	do  echo ========== $DIR $MAX $NTHREADS ========== >> $OUT
	    for COUNT in `seq 1 3`
	    do  $TIME bin/sieve $MAX $NTHREADS >/dev/null 2>>$OUT
	    done
	    echo "" >> $OUT
	done
    done
    cd ..
done

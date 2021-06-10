default: build bin sieve keep
 
build bin:
	mkdir $@

clean:
	\rm -f build/*.o bin/sieve bin/keep
 
sieve.o: sieve.cpp
	g++ -fopenmp -Wall -fomit-frame-pointer -O2 -c sieve.cpp -o build/sieve.o
 
sieve: sieve.o
	g++ -fopenmp -o bin/sieve build/sieve.o
 
keep.o: sieve.cpp
	g++ -fopenmp -Wall -fomit-frame-pointer -O2 -DKEEP_PRIMES -c sieve.cpp -o build/keep.o
 
keep: keep.o
	g++ -fopenmp -o bin/keep build/keep.o

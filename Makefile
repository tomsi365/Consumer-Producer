all : test

ex3_q1_given.o : ex3_q1_given.c ex3_q1_given.h
	echo 'compailing...'
	gcc -pthread -Wall ex3_q1_given.c -c -o ex3_q1_given.o
	
ex3_q1.o : ex3_q1.c ex3_q1.h 
	echo 'compailing...'
	gcc -pthread -Wall ex3_q1.c -c -o ex3_q1.o
		
ex3_q1 : ex3_q1.o ex3_q1_given.o
	echo 'linking...'
	gcc -pthread -Wall ex3_q1.o ex3_q1_given.o -lm -o ex3_q1
	
clean :
	rm -v *.o *.log ex3_q1
	
test : ex3_q1 
	./ex3_q1 > out.log 2> err.log 
	
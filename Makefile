all: tsvToBin.c pandemicStats.c
	gcc -c tsvToBin.c
	gcc -o tsvToBin tsvToBin.o
	gcc -c pandemicStats.c
	gcc -o pandemicStats pandemicStats.o


tsvToBin: tsvToBin.o
	gcc -o tsvToBin tsvToBin.o

tsvToBin.o: tsvToBin.c
	gcc -c tsvToBin.c

pandemicStats: pandemicStats.o
	gcc -o pandemicStats pandemicStats.o

pandemicStats.o: pandemicStats.c
	gcc -c pandemicStats.c

clean:
	rm *.o
	rm *.bin

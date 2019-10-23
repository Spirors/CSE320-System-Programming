.PHONY: progfib progcnt progqsort part3 clean

all: main.x

main.x:
	gcc src/main.c src/defs.c -I./inc -o museum_overseer -lpthread
progfib:
	gcc part2/fib.c src/defs.c -I./inc -o hw4_progfib -lpthread
progcnt:
	gcc part2/cnt.c src/defs.c -I./inc -o hw4_progcnt -lpthread
progqsort:
	gcc part2/qsort.c src/defs.c -I./inc -o hw4_progqsort -lpthread
part3:
	gcc part3/src/main.c part3/src/artist_ds.c src/defs.c -I./inc -I./part3/inc -o artist_manager -lpthread
clean:
	-rm hw4_progfib
	-rm hw4_progcnt
	-rm hw4_progqsort
	-rm artist_manager
	-rm museum_overseer

all: main.x

main.x:
	gcc src/artist.c src/defs.c -I./inc -o artist
	gcc src/main.c src/defs.c src/artist_ds.c -I./inc -o artist_manager

clean:
	rm artist
	rm artist_manager

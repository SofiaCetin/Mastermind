all:
	gcc -Wall src/main.c src/gui.c src/script.c -o mastermind.out \
	$(shell pkg-config --cflags --libs sdl3 sdl3-ttf)


clean:
	rm -f mastermind.out
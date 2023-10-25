CC=gcc

CFLAGS=-I raylib/build/raylib/include/
LIBS=-L raylib/build/raylib/ -lraylib -lm -ldl -llua

.PHONY: all
all: build/main

build/main: src/*
	mkdir -p build
	$(CC) src/main.c $(CFLAGS) $(LIBS) -o build/main

.PHONY: run
run: build/main
	echo "------"
	build/main

.PHONY: watch
watch:
	find src/ mods/ | entr -s "make run"

.PHONY: clean
clean:
	rm -rf build

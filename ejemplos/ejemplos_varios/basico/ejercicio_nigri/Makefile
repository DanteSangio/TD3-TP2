all:
	gcc ./src/*.c -I./inc -o exe -Wall -pthread

debug:
	gcc ./src/*.c -I./inc -o exe -Wall -pthread -DDEBUG_CHILD

execute:
	./exe 10 2 10 5
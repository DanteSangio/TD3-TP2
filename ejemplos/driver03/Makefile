# crosscompiler
CC = /home/dante/Imagen_BBB/gcc-linaro-6.4.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

SOURCES = $(wildcard ./src/*.c)
INCLUDES = $(wildcard ./inc/*.h)
OBJETOS = $(subst src/,obj/,$(subst .c,.o,$(SOURCES)))
EJECUTABLE = 02_better_driver.c
OPCIONES = -Wall
DESTINO = ~





.PHONY: all
all: compilar copiar_a_BBB





.PHONY: compilar
compilar: $(OBJETOS)
	$(CC) $^ -Iinc -o $(EJECUTABLE) $(OPCIONES)





.PHONY: copiar_a_BBB
copiar_a_BBB:
	scp exe debian@192.168.7.2:$(DESTINO)
	ssh debian@192.168.7.2


obj/%.o: src/%.c $(INCLUDES)
	$(CC) -c $(SOURCES) -Iinc -o $@





.PHONY: crearEstructura
crearEstructura:
	@echo ''
	@echo '-----> Creando estructura de carpetas'

	mkdir -p src inc obj





.PHONY: clean
clean:
	rm $(OBJETOS) $(EJECUTABLE)
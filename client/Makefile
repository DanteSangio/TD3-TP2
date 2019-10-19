# ******************************************************************** #
# **************************** Definitions *************************** #
# ******************************************************************** #

# crosscompiler
CC = /home/dante/Imagen_BBB/gcc-linaro-6.4.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

SOURCES = $(wildcard ./src/*.c)
INCLUDES = $(wildcard ./inc/*.h)
OBJETOS = $(subst src/,obj/,$(subst .c,.o,$(SOURCES)))
EJECUTABLE = ./bin/client
OPCIONES = -pthread -Wall


# bbb stuff
PASSWORD = temppwd
IP = 192.168.7.2


# servers : ip port frecuency
SERVER1 = 127.0.0.1 1234 6
SERVER2 = 127.0.0.1 2234 6
SERVER3 = 127.0.0.1 3234 6




# ******************************************************************** #
# ******************************* Rules ****************************** #
# ******************************************************************** #

.PHONY: all
all: compilar copy


.PHONY: compilar
compilar: $(OBJETOS)
	$(CC) $^ -Iinc -o $(EJECUTABLE) $(OPCIONES)


obj/%.o: src/%.c $(INCLUDES)
	$(CC) -c $< -Iinc -o $@


.PHONY: crearEstructura
crearEstructura:
	@echo ''
	@echo '-----> Creando estructura de carpetas'

	mkdir -p src inc obj


.PHONY: clean
clean:
	rm $(OBJETOS) $(EJECUTABLE)


.PHONY: copy
copy:
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/client/*"
	sshpass -p $(PASSWORD) scp -r ./* debian@$(IP):~/dante/client/


.PHONY: copyandlog
copyandlog:
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/client/*"
	sshpass -p $(PASSWORD) scp -r ./* debian@$(IP):~/dante/client/
	sshpass -p $(PASSWORD) ssh debian@$(IP)


.PHONY: log
log:
	sshpass -p $(PASSWORD) ssh debian@$(IP)


.PHONY: run_client
run_client:
	clear
	./bin/client $(SERVER1) $(SERVER2) $(SERVER3)




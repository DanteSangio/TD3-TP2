# ******************************************************************** #
# **************************** Definitions *************************** #
# ******************************************************************** #

# crosscompiler
CC = /home/dante/Imagen_BBB/gcc-linaro-6.4.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

SOURCES = $(wildcard ./src/*.c)
INCLUDES = $(wildcard ./inc/*.h)
OBJETOS = $(subst src/,obj/,$(subst .c,.o,$(SOURCES)))
EJECUTABLE = ./bin/server
OPCIONES = -pthread -Wall
#KBUILD_EXTMOD

# bbb stuff
PASSWORD = temppwd
IP = 192.168.7.2


# server : ip port driver-configuration
# driver configuration:
#		1 = axis x
#		2 = axis y
#		3 = axis z
#		4 = all axis
SERVER1 = 1234 1
SERVER2 = 2234 2
SERVER3 = 3234 3
SERVER4 = 3234 4

# ******************************************************************** #
# ******************************* Rules ****************************** #
# ******************************************************************** #

.PHONY: all
all: compilar


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
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/server/*"
	sshpass -p $(PASSWORD) scp -r ./* debian@$(IP):~/dante/server/


.PHONY: copyandlog
copyandlog:
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/server/*"
	sshpass -p $(PASSWORD) scp -r ./* debian@$(IP):~/dante/server/
	sshpass -p $(PASSWORD) ssh debian@$(IP)


.PHONY: log
log:
	sshpass -p $(PASSWORD) ssh debian@$(IP)


.PHONY: run_server1
run_server1:
	clear
	./bin/server $(SERVER1)


.PHONY: run_server2
run_server2:
	clear
	./bin/server $(SERVER2)


.PHONY: run_server3
run_server3:
	clear
	./bin/server $(SERVER3)


.PHONY: run_server4
run_server4:
	clear
	./bin/server $(SERVER4)




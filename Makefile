# ******************************************************************** #
# **************************** Definitions *************************** #
# ******************************************************************** #

# bbb_stuff
PASSWORD = temppwd
IP = 192.168.7.2

# crosscompiler
CC = /home/dante/Imagen_BBB/gcc-linaro-6.4.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

# client
CLIENT_SOURCES = $(wildcard ./client/src/*.c)
CLIENT_INCLUDES = $(wildcard ./client/inc/*.h)
CLIENT_OBJETOS = $(subst src/,obj/,$(subst .c,.o,$(CLIENT_SOURCES)))
CLIENT_EJECUTABLE = ./client/bin/client
CLIENT_OPCIONES = -pthread -Wall

# server
SERVER_SOURCES = $(wildcard ./server/src/*.c)
SERVER_INCLUDES = $(wildcard ./server/inc/*.h)
SERVER_OBJETOS = $(subst src/,obj/,$(subst .c,.o,$(SERVER_SOURCES)))
SERVER_EJECUTABLE = ./server/bin/server
SERVER_OPCIONES = -pthread -Wall

# driver test
TEST = $(wildcard ./driver/test/*.c)
TEST_EJECUTABLE = ./driver/test/td3_spi_test
TEST_OPCIONES = -Wall


ALL_INCLUDES = -I./client/inc/ -I./server/inc/


PASSWORD = temppwd
IP = 192.168.7.2


# ******************************************************************** #
# ******************************* Rules ****************************** #
# ******************************************************************** #

.PHONY: all
all: clc clientcompile servercompile testcompile copy

.PHONY: everything
everything: clc clientclean serverclean clientcompile servercompile testcompile copy




# compile

.PHONY: compile
compile: clc clientcompile servercompile testcompile


.PHONY: compileandclean
compileandclean: clc clientclean serverclean clientcompile servercompile testcompile


.PHONY: clientcompile
clientcompile: $(CLIENT_OBJETOS)
	@echo '-----> Compiling client'
	$(CC) $^ $(ALL_INCLUDES) -o $(CLIENT_EJECUTABLE) $(CLIENT_OPCIONES)
	@echo ''
	@echo ''


.PHONY: servercompile
servercompile: $(SERVER_OBJETOS)
	@echo '-----> Compiling server'
	$(CC) $^ $(ALL_INCLUDES) -o $(SERVER_EJECUTABLE) $(SERVER_OPCIONES)
	@echo ''
	@echo ''


.PHONY: testcompile
testcompile:
	@echo '-----> Compiling test'
	$(CC) $(TEST) -o $(TEST_EJECUTABLE) $(TEST_OPCIONES)
	@echo ''
	@echo ''


client/obj/%.o: client/src/%.c $(CLIENT_INCLUDES) $(SERVER_INCLUDES)
	$(CC) -c $< $(ALL_INCLUDES) -o $@

server/obj/%.o: server/src/%.c $(CLIENT_INCLUDES) $(SERVER_INCLUDES)
	$(CC) -c $< $(ALL_INCLUDES) -o $@




# clean

.PHONY: clientclean
clientclean:
	@echo '-----> Cleaning client'
	rm $(CLIENT_OBJETOS) $(CLIENT_EJECUTABLE)
	@echo ''
	@echo ''


.PHONY: serverclean
serverclean:
	@echo '-----> Cleaning server'
	rm $(SERVER_OBJETOS) $(SERVER_EJECUTABLE)
	@echo ''
	@echo ''




.PHONY: clc
clc:
	clear




# copy

.PHONY: copy
copy:
	@echo '-----> Listo para copiar'
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/client/*"
	sshpass -p $(PASSWORD) scp -r ./client/* debian@$(IP):~/dante/client/
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/server/*"
	sshpass -p $(PASSWORD) scp -r ./server/* debian@$(IP):~/dante/server/
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/driver/*"
	sshpass -p $(PASSWORD) scp -r ./driver/* debian@$(IP):~/dante/driver/


.PHONY: copyandlog
copyandlog:
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/client/*"
	sshpass -p $(PASSWORD) scp -r ./client/* debian@$(IP):~/dante/client/
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/server/*"
	sshpass -p $(PASSWORD) scp -r ./server/* debian@$(IP):~/dante/server/
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf /home/debian/dante/driver/*"
	sshpass -p $(PASSWORD) scp -r ./driver/* debian@$(IP):~/dante/driver/
	sshpass -p $(PASSWORD) ssh debian@$(IP)


.PHONY: log
log:
	sshpass -p $(PASSWORD) ssh debian@$(IP)




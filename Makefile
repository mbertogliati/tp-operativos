SRC = ../src/*.c
INCLUDE = -I../include/ -I../../shared/include/ -I../../shared/include/estructuras -I../../shared/include/sockets
SHARED_SRC = ../src/estructuras/*.c ../src/sockets/*.c
SHARED_INCLUDE = -I../include/ -I../include/estructuras/ -I../include/sockets/

CPU_OBJS = cpu/obj/*.o
CONSOLA_OBJS = consola/obj/*.o
KERNEL_OBJS = kernel/obj/*.o
MEMORIA_OBJS = memoria/obj/*.o
SHARED_OBJS = shared/obj/*.o
LINK_INCLUDE = -L../include/ -L../../shared/include
EJECUTABLES = cpu/cpu.out kernel/kernel.out memoria/memoria.out consola/consola.out *.out

CC = gcc
CFLAGS = -O0 -g -Wall -pthread
LINK_FLAGS = -Xlinker -lcommons -lpthread -lreadline -lm

#Compila y linkea todos los modulos y genera todos los ejecutables:
all: cpu consola memoria kernel

#Genera ejecutable cpu:
cpu: compile_cpu
			gcc $(CFLAGS) -o cpu.out $(CPU_OBJS) $(SHARED_OBJS) $(LINK_FLAGS)

#Genera ejecutable kernel:
kernel: compile_kernel
			gcc $(CFLAGS) -o kernel.out $(KERNEL_OBJS) $(SHARED_OBJS) $(LINK_FLAGS)

#Genera ejecutable memoria:
memoria: compile_memoria
			gcc $(CFLAGS) -o memoria.out $(MEMORIA_OBJS) $(SHARED_OBJS) $(LINK_FLAGS)

#Genera ejecutable consola:
consola: compile_consola
			gcc $(CFLAGS) -o consola.out $(CONSOLA_OBJS) $(SHARED_OBJS) $(LINK_FLAGS)

#Genera objetos consola:
compile_consola:
			cd consola/obj;gcc $(CFLAGS) $(INCLUDE) -c $(SRC)

#Genera objetos memoria:
compile_memoria: compile_shared
			cd memoria/obj;gcc $(CFLAGS) $(INCLUDE) -c $(SRC)

#Genera objetos kernel:
compile_kernel:compile_shared
			cd kernel/obj;gcc $(CFLAGS) $(INCLUDE) -c $(SRC)	

#Genera objetos cpu:
compile_cpu: compile_shared
			cd cpu/obj;gcc $(CFLAGS) $(INCLUDE) -c $(SRC)

#Genera objetos shared:
compile_shared:
			cd shared/obj;gcc $(CFLAGS) $(SHARED_INCLUDE) -c $(SHARED_SRC)

#Borra todos los objetos y ejecutables:
clean:
	rm -f $(CPU_OBJS) $(KERNEL_OBJS) $(MEMORIA_OBJS) $(CONSOLA_OBJS) $(SHARED_OBJS) $(EJECUTABLES)
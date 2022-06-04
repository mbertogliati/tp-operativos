#include "../include/kernel.h"

void funcion_prueba_checkpoint(){
	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
	config = config_create("kernel.config");
	inicializar_colas_checkpoint();

	sem_init(&mutex_new, 0, 1);
	sem_init(&mutex_suspendido, 0, 1);
	sem_init(&mutex_exit, 0, 1);
	sem_init(&mutex_memoria, 0, 1);

	sem_init(&procesos_en_new, 0, 0);
	sem_init(&procesos_en_exec, 0, 0);
	sem_init(&procesos_en_suspendido, 0, 0);
	sem_init(&procesos_en_exit, 0, 0);

	sem_init(&disponible_para_exec, 0, 1);

	pthread_t thread_new, thread_exec,thread_suspendido, thread_exit;
	pthread_t thread_consola;

	log_info(logger, "Creo thread de new");
	pthread_create(&thread_new, NULL, &thread_de_new, NULL);

	log_info(logger, "Creo thread de exec");
	pthread_create(&thread_new, NULL, &thread_de_exec, NULL);

	log_info(logger, "Creo thread de suspendido");
	pthread_create(&thread_new, NULL, &thread_de_suspendido, NULL);

	log_info(logger, "Creo thread de exit");
	pthread_create(&thread_new, NULL, &thread_de_exit, NULL);


	log_info(logger, "Creo thread de consola");
	pthread_create(&thread_consola, NULL, &conectar_consola, puerto_escucha());
}

void inicializar_colas_checkpoint(){
	queue_new = queue_create();
	exec = queue_create();
	suspendido = queue_create();
	queue_exit = queue_create();
}


void *thread_de_new(){
	while(1){
		sem_wait(&disponible_para_exec);
		sem_wait(&procesos_en_new);
		sem_wait(&mutex_new);
		t_pcb *proceso = queue_pop(queue_new);
		log_info(logger, "Muevo el proceso %d de new a exec", proceso->id);
		queue_push(exec, proceso);
		sem_post(&mutex_new);
		sem_post(&procesos_en_exec);
	}
}


void *thread_de_exec(){
	while(1){
		sem_wait(&procesos_en_exec);



		//Para demostrar que se puede suspender un proceso, un proceso que regresa de cpu tiene
		//un 50% de chances de ir a suspendido
		if(rand() / 2 == 0){
			sem_wait(&mutex_new);
			t_pcb *proceso = queue_pop(exec);
			log_info(logger, "Muevo el proceso %d de exec a new", proceso->id);
			queue_push(queue_new, proceso);
			sem_post(&mutex_new);
			sem_post(&procesos_en_new);
			sem_post(&disponible_para_exec);
		}else{
			sem_wait(&mutex_suspendido);
			t_pcb *proceso = queue_pop(exec);
			log_info(logger, "Muevo el proceso %d de exec a suspendido", proceso->id);
			queue_push(suspendido, proceso);
			sem_post(&mutex_suspendido);
			sem_post(&procesos_en_suspendido);
			sem_post(&disponible_para_exec);

		}
	}
}

void *thread_de_suspendido(){
	while(1){
		sem_wait(&procesos_en_suspendido);
		sem_wait(&mutex_suspendido);
		t_pcb *proceso = queue_pop(suspendido);
		sem_post(&mutex_suspendido);
		sem_wait(&mutex_memoria);
		suspender_proceso_memoria(proceso->tabla_paginas);
		sem_post(&mutex_memoria);
		sleep(20);
		log_info(logger, "Muevo el proceso %d de suspendido a new", proceso->id);
		sem_wait(&mutex_new);
		queue_push(queue_new, proceso);
		sem_post(&mutex_new);

	}
}

void *thread_de_exit(){
	while(1){
		sem_wait(&procesos_en_exit);
		sem_wait(&mutex_exit);
		t_pcb *proceso = queue_pop(queue_exit);
		sem_wait(&mutex_exit);
		log_info(logger, "Finalizo el proceso %d", proceso->id);
		sem_wait(&mutex_memoria);
		finalizar_proceso_memoria(proceso->tabla_paginas);
		sem_post(&mutex_memoria);
	}
}


int main() {
	funcion_prueba_checkpoint();
//	logger = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
//	config = config_create("kernel.config");
//
//	inicializar_colas();
//
//	conectar_memoria(ip_memoria(), puerto_memoria());
//	conectar_consola(puerto_escucha());
//	conectar_cpu(ip_cpu(), puerto_cpu_dispatch(), puerto_cpu_interrupt);
//
//	desconectar_memoria();
//
//	log_destroy(logger);
//	config_destroy(config);
//
//	return EXIT_SUCCESS;
}

#include "../include/kernel.h"

void funcion_prueba_checkpoint(){
	memoria_log = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
	config = config_create("kernel.config");
	inicializar_colas_checkpoint();
	conectar_memoria(ip_memoria(), puerto_memoria());
	conectar_cpu(ip_cpu(), puerto_cpu_dispatch(), puerto_cpu_interrupt());

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

	log_info(memoria_log, "Creo thread de new");
	pthread_create(&thread_new, NULL, &thread_de_new, NULL);

	log_info(memoria_log, "Creo thread de exec");
	pthread_create(&thread_exec, NULL, &thread_de_exec, NULL);

	log_info(memoria_log, "Creo thread de suspendido");
	pthread_create(&thread_suspendido, NULL, &thread_de_suspendido, NULL);

	log_info(memoria_log, "Creo thread de exit");
	pthread_create(&thread_exit, NULL, &thread_de_exit, NULL);

	log_info(memoria_log, "Creo thread de consola");
	pthread_create(&thread_consola, NULL, &conectar_consola, puerto_escucha());

	pthread_join(thread_new, NULL);
	pthread_join(thread_exec, NULL);
	pthread_join(thread_suspendido, NULL);
	pthread_join(thread_exit, NULL);
	pthread_join(thread_consola, NULL);

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
		log_info(memoria_log, "Muevo el proceso %d de new a exec", proceso->id);
		queue_push(exec, proceso);
		sem_post(&mutex_new);
		sem_post(&procesos_en_exec);
	}
}


void *thread_de_exec(){
	t_pcb *proceso;
	t_pcb_con_milisegundos proceso_recibido;
	while(1){
		sem_wait(&procesos_en_exec);
		proceso = queue_pop(exec);
		sem_wait(&mutex_new);

		//Para demostrar que se puede suspender un proceso, un proceso que regresa de cpu tiene
		//un 50% de chances de ir a suspendido
		if(rand() % 2 == 0){
			
			//Se manda a ejecutar a la cpu
			ejecutar_proceso(proceso);
			liberar_pcb(proceso);

			proceso_recibido = recibir_proceso();

			if(proceso_recibido.pcb->cant_instrucciones == proceso_recibido.pcb->program_counter){
				sem_wait(&mutex_exit);
				queue_push(queue_exit, proceso_recibido.pcb);
				sem_post(&mutex_exit);
				sem_post(&procesos_en_exit);
			}
			else{
				if(proceso_recibido.milisegundos == 0){
					proceso = proceso_recibido.pcb;
					log_info(memoria_log, "El proceso %d se ha recibido correctamente", proceso->id);
					log_info(memoria_log, "Muevo el proceso %d de exec a new", proceso->id);
					queue_push(queue_new, proceso);
					sem_post(&mutex_new);
					sem_post(&procesos_en_new);
					sem_post(&disponible_para_exec);
				}
				else{
					sem_wait(&mutex_suspendido);
					log_info(memoria_log, "Operacion de IO por &dms solicitada", proceso_recibido);
					log_info(memoria_log, "Muevo el proceso %d de exec a suspendido", proceso_recibido.pcb->id);
					queue_push(suspendido, &proceso_recibido);
					sem_post(&mutex_suspendido);
					sem_post(&procesos_en_suspendido);
					sem_post(&disponible_para_exec);	
				}
			}

		}else{
			proceso_recibido.pcb = proceso;
			proceso_recibido.milisegundos = 10;
			sem_wait(&mutex_suspendido);
			log_info(memoria_log, "Muevo el proceso %d de exec a suspendido", proceso->id);
			queue_push(suspendido, &proceso_recibido);
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
		t_pcb_con_milisegundos *proceso = queue_pop(suspendido);
		sem_post(&mutex_suspendido);
		sem_wait(&mutex_memoria);
		suspender_proceso_memoria(proceso->pcb->tabla_paginas);
		sem_post(&mutex_memoria);
		log_info(memoria_log, "IO por &dms", proceso->milisegundos);
		sleep(proceso->milisegundos);
		log_info(memoria_log, "Muevo el proceso %d de suspendido a new", proceso->pcb->id);
		sem_wait(&mutex_new);
		queue_push(queue_new, proceso->pcb);
		sem_post(&mutex_new);

	}
}

void *thread_de_exit(){
	while(1){
		sem_wait(&procesos_en_exit);
		sem_wait(&mutex_exit);
		t_pcb *proceso = queue_pop(queue_exit);
		sem_post(&mutex_exit);
		log_info(memoria_log, "Finalizo el proceso %d", proceso->id);
		sem_wait(&mutex_memoria);
		finalizar_proceso_memoria(proceso->tabla_paginas);
		sem_post(&mutex_memoria);
	}
}


int main() {
	funcion_prueba_checkpoint();
//	memoria_log = log_create("kernel.log", "KERNEL", 1, LOG_LEVEL_DEBUG);
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
//	log_destroy(memoria_log);
//	config_destroy(config);
//
	return EXIT_SUCCESS;
}
void ejecutar_proceso(t_pcb* pcb){

	log_info(memoria_log, "Enviando proceso a ejecutarse...");
	enviar_pcb(pcb, socket_dispatch, 0);

}
t_pcb_con_milisegundos recibir_proceso(){

	t_pcb_con_milisegundos proceso_para_devolver;
	t_pcb* pcb;
	t_buffer* buffer = malloc(sizeof(t_buffer));
	int IO;
	buffer->stream = recibir_buffer(&(buffer->size), socket_dispatch);

	IO = sacar_de_buffer(buffer, sizeof(int)); //Saco el tiempo de IO que se lo pase como "codigo de operacion" APROVECHA EL BUG XD
	sacar_de_buffer(buffer, sizeof(int));//Saco el tamaño porque no lo necesito

	pcb = desempaquetar_pcb(buffer->stream);
	proceso_para_devolver.milisegundos = IO;
	proceso_para_devolver.pcb = pcb;
	free(buffer);

	return proceso_para_devolver;
}

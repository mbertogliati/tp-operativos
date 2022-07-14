#include "../include/colas.h"

void inicializar_estructuras() {
	sem_init(&mlog, 0, 1);
	log_protegido(string_from_vformat("Inicializando estructuras   %d.", 99));

	//Colas
	new_queue = queue_create();
	ready = list_create();
	bloqueado_queue = queue_create();
	bloqueado_tiempo = queue_create();
	suspendido_bloqueado = queue_create();
	suspendido_ready = queue_create();
	suspendido_tiempo = queue_create();
	exit_queue = queue_create();
	log_protegido("Colas creadas.");

	//Counters
	new_counter = 0;
	suspendido_counter = 0;
	id_counter = 1;
	log_protegido("Contadores inicializados.");

	//Semaforos
	sem_init(&nivel_multiprogramacion, 0, grado_multiprogramacion());
	sem_init(&procesos_en_ready, 0, 0);
	sem_init(&procesos_en_exit, 0, 0);
	sem_init(&ready_disponible, 0, 0);
	sem_init(&suspendido, 0, 0);
	sem_init(&bloqueado, 0, 0);
	log_protegido("Semaforos inicializados.");

	//Semaforos Mutex
	sem_init(&mnew, 0, 1);
	sem_init(&mnew_counter, 0, 1);
	sem_init(&mid_counter, 0, 1);
	sem_init(&mready, 0, 1);
	sem_init(&mexit, 0, 1);
	sem_init(&msuspendido_counter, 0, 1);
	sem_init(&mbloqueado, 0, 1);
	sem_init(&mbloqueado_tiempo, 0, 1);
	sem_init(&msuspendido_bloqueado, 0, 1);
	sem_init(&msuspendido_tiempo, 0, 1);
	sem_init(&msuspendido_ready, 0, 1);
	log_protegido("Semaforos Mutex inicializados.");
}

int inicializar_threads(){
	pthread_t hilo_ready, hilo_execute, hilo_blocked, hilo_suspendido_blocked, hilo_exit;

	pthread_create(&hilo_ready, NULL, &thread_ready, NULL);
	pthread_create(&hilo_execute, NULL, &thread_execute, NULL);
	pthread_create(&hilo_blocked, NULL, &thread_blocked, NULL);
	pthread_create(&hilo_suspendido_blocked, NULL, &thread_suspendido_blocked, NULL);
	pthread_create(&hilo_exit, NULL, &thread_exit, NULL);

	pthread_join(hilo_ready, NULL);
	pthread_join(hilo_execute, NULL);
	pthread_join(hilo_blocked, NULL);
	pthread_join(hilo_suspendido_blocked, NULL);
	pthread_join(hilo_exit, NULL);

	return EXIT_SUCCESS;
}

void agregar_a_cola(t_queue *cola, t_pcb *pcb, sem_t semaforo_mutex){
	sem_wait(&semaforo_mutex);
	queue_push(cola, pcb);
	sem_post(&semaforo_mutex);
}

t_pcb* sacar_de_cola(t_queue *cola, sem_t semaforo_mutex){
	sem_wait(&semaforo_mutex);
	t_pcb* pcb = queue_pop(cola);
	sem_post(&semaforo_mutex);
	return pcb;
}

void log_protegido(char* mensaje){
	sem_wait(&mlog);
	log_info(log_kernel, mensaje, arguments);
	sem_post(&mlog);
}

// new
void agregar_a_new(t_pcb *pcb) {
	sem_wait(&mid_counter);
	pcb->id = id_counter;
	id_counter++;
	sem_post(&mid_counter);
	pcb->program_counter = 0;
	pcb->est_rafaga = estimacion_inicial();
	pcb->rafaga_inicial = 0;

	sem_wait(&mnew);
	queue_push(new_queue, pcb);
	sem_post(&mnew);
	sem_wait(&mnew_counter);
	new_counter++;
	sem_post(&mnew_counter);
	log_protegido(string_from_vformat("CONSOLA:Se agrego el pcb %d a new", pcb->id));

	sem_post(&ready_disponible);
}


// ready-fifo
void agregar_a_ready_fifo(t_pcb *pcb) {
	//sem_wait(&procesos);
	//sem_wait(&grado_multip);

	//pthread_mutex_lock(&mready);
	sem_wait(&mready);
	list_add(ready, pcb);
	sem_post(&mready);
	//pthread_mutex_unlock(&mready);
}

// ready-sjf
double calcular_estimacion(double real, t_pcb *pcb) {
	pcb->est_rafaga = pcb->est_rafaga * alfa() + real * (1 - alfa());
	log_protegido(string_from_vformat("Se estima rafaga de %lf para el proceso %d.", pcb->est_rafaga, pcb->id));
	return pcb->est_rafaga;
}

int menor_estimacion(t_pcb *pcb1, t_pcb * pcb2) {
	return pcb1->est_rafaga < pcb2->est_rafaga;
}

void agregar_a_ready_sjf(t_pcb *pcb) {
	//sem_wait(&procesos);
	//sem_wait(&grado_multip);

	//pthread_mutex_lock(&mready);
	sem_wait(&mready);
	list_add_sorted(ready, pcb, (void *) menor_estimacion);
	sem_wait(&mready);
	//pthread_mutex_unlock(&mready);

	//TODO Avisar por interrupt
	log_protegido("Se da aviso de interrupt a CPU.");
	bool interrupcion = true;
	send(socket_interrupt, &interrupcion, sizeof(bool), 0);
	

}

// ready
void agregar_a_ready(t_pcb *pcb) {
	//enviar_mensaje_memoria("init", 5);
	//pcb->tabla_paginas = (int) recibir_mensaje_memoria();
	if(!strcmp("FIFO", algoritmo_planificacion()))
		agregar_a_ready_fifo(pcb);
	if(!strcmp("SJF", algoritmo_planificacion()))
		agregar_a_ready_sjf(pcb);
}

t_pcb* quitar_de_ready(){
	sem_wait(&mready);
	t_pcb* pcb = list_remove(ready, 0);
	sem_post(&mready);
	return pcb;
}

//false = new, true = suspendido-ready
bool disponible_para_ready(){
	bool respuesta;
	sem_wait(&ready_disponible);

	//Chequea si hay un proceso en new
	sem_wait(&mnew_counter);
	if(new_counter > 0){
		respuesta = false;
	}
	sem_post(&mnew_counter);

	//Chequea si hay un proceso en suspendido_ready
	sem_wait(&msuspendido_counter);
	if(suspendido_counter > 0){
		respuesta = true;
	}
	sem_post(&msuspendido_counter);

	return respuesta;
}

void *thread_ready(){
	while(1){
		//Revisar semaforo de nivel multiprogramacion
		sem_wait(&nivel_multiprogramacion);
		log_protegido("READY:El nivel de multiprogramacion permite agregar un proceso a ready.");

		if(disponible_para_ready()){
			//Suspendido-ready
			log_protegido("READY:Hay un proceso disponible en suspendido-ready.");
			sem_wait(&msuspendido_counter);
			suspendido_counter--;
			sem_post(&msuspendido_counter);
			t_pcb *pcb = sacar_de_cola(suspendido_ready, msuspendido_ready);
			pcb->tabla_paginas = agregar_proceso_memoria(pcb->id, pcb->tamanio);
			agregar_a_ready(pcb);
			log_protegido("READY:Se agrego el proceso %d a ready.", pcb->id);
		}else{
			//New
			log_protegido("READY:Hay un proceso disponible en new.");
			sem_wait(&mnew_counter);
			new_counter--;
			sem_post(&mnew_counter);
			t_pcb *pcb = sacar_de_cola(new_queue, mnew);
			pcb->tabla_paginas = agregar_proceso_memoria(pcb->id, pcb->tamanio);
			agregar_a_ready(pcb);
			log_protegido("READY:Se agrego el proceso %d a ready.", pcb->id);
		}
		//Avisa que hay procesos en ready
		sem_post(&procesos_en_ready);
	}
}

//execute
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


void *thread_execute(){
	while(1){
		t_pcb_con_milisegundos proceso_recibido;
		time_t tiempo_envio, tiempo_retorno;
		sem_wait(&procesos_en_ready);
		log_info(log_kernel, "EXECUTE:Hay procesos en ready para ejecutar.");

		t_pcb* pcb = quitar_de_ready();
		log_protegido("EXECUTE:Se mueve a ejecutar el proceso %d.", pcb->id);

		//Enviar a cpu;
		enviar_pcb(pcb, socket_dispatch, 0);
		time(&tiempo_envio);
		log_protegido("EXECUTE:Proceso enviado a CPU para su ejecucion.");
		//Eliminar pcb
		liberar_pcb(pcb);
		//Recibir nuevo pcb con milisegundos
		proceso_recibido = recibir_proceso();
		time(&tiempo_retorno);
		log_protegido("EXECUTE:Proceso recibido de CPU.");


		if(proceso_recibido.pcb->program_counter < proceso_recibido.pcb->cant_instrucciones){
			//Interrupcion
			if(proceso_recibido.milisegundos == 0){
				log_protegido("EXECUTE:Proceso recibido por interrupcion.");
				if(pcb->rafaga_inicial == 0){
					pcb->rafaga_inicial = pcb->est_rafaga;
					pcb->est_rafaga = pcb->est_rafaga - (tiempo_retorno - tiempo_envio);
				}else{
					pcb->est_rafaga = pcb->est_rafaga - (tiempo_retorno - tiempo_envio);
				}

				agregar_a_ready(proceso_recibido.pcb);
				log_protegido("EXECUTE:Se agrego el proceso a ready.");
				sem_post(&procesos_en_ready);
			}else{
				//Espera
				log_protegido("EXECUTE:Proceso recibido por I/O.");

				//Hago la estimacion de la proxima rafaga
				if(pcb->rafaga_inicial == 0){
					calcular_estimacion(tiempo_retorno - tiempo_envio, pcb);
				}else{
					int real = pcb->est_rafaga;
					pcb->est_rafaga = pcb->rafaga_inicial;
					real = pcb->est_rafaga - real + (tiempo_retorno - tiempo_envio);
					calcular_estimacion(real, pcb);
				}
				pcb->rafaga_inicial = 0;

				agregar_a_cola(bloqueado_queue, proceso_recibido.pcb, mbloqueado);
				sem_wait(&mbloqueado_tiempo);
				queue_push(bloqueado_tiempo, proceso_recibido.milisegundos);
				sem_post(&mbloqueado_tiempo);
				log_protegido("EXECUTE:Se agrego el proceso a bloqueado.");
				sem_post(&bloqueado);
			}
		}else{
			//Exit
			log_protegido("EXECUTE:Proceso finalizo sus instrucciones.");
			agregar_a_cola(exit_queue, proceso_recibido.pcb, mexit);
			log_protegido("EXECUTE:Se agrego el proceso a exit.");
			sem_post(&procesos_en_exit);
			sem_post(&nivel_multiprogramacion);
		}

	}
}

//blocked
int esperar_bloqueado(int tiempo_espera){
	if(tiempo_espera > tiempo_max_bloqueado())
		sleep(0.001 * tiempo_max_bloqueado());
	else
		sleep(0.001 * tiempo_espera);

	return (tiempo_espera - tiempo_max_bloqueado());
}

void *thread_blocked(){
	while(1){
		sem_wait(&bloqueado);
		t_pcb* pcb = sacar_de_cola(bloqueado_queue, mbloqueado);
		sem_wait(&mbloqueado_tiempo);
		int espera_restante = queue_pop(bloqueado_tiempo);
		sem_post(&mbloqueado_tiempo);
		log_protegido("BLOCKED:Realizando espera de %d milisegundos para el proceso %d en bloqueado.", espera_restante, pcb->id);
		espera_restante = esperar_bloqueado(espera_restante);

		if(espera_restante > 0){
			log_protegido("BLOCKED:Espera mayor a la permitida en bloqueado, suspendiendo el proceso.");
			suspender_proceso_memoria(pcb->tabla_paginas);
			agregar_a_cola(suspendido_bloqueado, pcb, msuspendido_bloqueado);
			sem_wait(&msuspendido_tiempo);
			queue_push(suspendido_tiempo, espera_restante);
			sem_post(&msuspendido_tiempo);
			sem_post(&suspendido);
			sem_post(&nivel_multiprogramacion);
		}else{
			log_protegido("BLOCKED:Espera en bloqueado completada, agregando a ready.");
			agregar_a_ready(pcb);
		}
	}
}

//suspendido-blocked
void *thread_suspendido_blocked(){
	while(1){
		sem_wait(&suspendido);
		t_pcb* pcb = sacar_de_cola(suspendido_bloqueado, msuspendido_bloqueado);
		sem_wait(&msuspendido_tiempo);
		int tiempo_espera = queue_pop(suspendido_tiempo);
		sem_post(&msuspendido_tiempo);

		log_protegido("SUSPENDIDO_BLOCKED:Realizando espera de %d milisegundos para el proceso %d en suspendido.", tiempo_espera, pcb->id);
		sleep(0.001 * tiempo_espera);

		log_protegido("SUSPENDIDO_BLOCKED:Espera en suspendido completada, agregando a suspendido-ready.");
		sem_wait(&msuspendido_ready);
		agregar_a_cola(suspendido_ready, pcb, msuspendido_ready);
		sem_post(&msuspendido_ready);

		sem_wait(&msuspendido_counter);
		suspendido_counter++;
		sem_post(&msuspendido_counter);

		sem_post(&ready_disponible);
	}
}


//exit
void *thread_exit(){
	while(1){
		sem_wait(&procesos_en_exit);

		t_pcb* pcb = sacar_de_cola(exit_queue, mexit);
		log_protegido("EXIT:Finalizando proceso %d.", pcb->id);
		finalizar_proceso_memoria(pcb->tabla_paginas);

		terminar_consola(pcb->fd);

		liberar_pcb(pcb);
		log_protegido("EXIT:Proceso finalizado.");
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "libfractal/fractal.h"
#include "additionals.h"

struct fractal ** buffer;

//Déclarations des mutex et semaphores
pthread_mutex_t mthread_buffer;
pthread_mutex_t mthread_closing;
sem_t empty;
sem_t full;
int main(int argc, char *argv[]){

	int genBMP = 0;
	int nb_max_threads = 1;

	char * fichier_out = argv[argc-1];
	char * fichiers_in[argc];


	/*Décomposition des arguments*/
	
	int nbf = 0;
	for (int i = 1; i < argc-1; i++){
		if (strcmp(argv[i], "-d") == 0)
			genBMP = 1;
		else if (strcmp(argv[i], "--maxthreads") == 0){
			i++;
			nb_max_threads = atoi(argv[i]);
		}else{
			fichiers_in[nbf]=argv[i];
			nbf++;
		}
	}
	for (int i = 0; i<nbf; i++){
		if(strcmp(fichiers_in[i],"-") == 0){
			printf("Entree standard\n"); //prendre en compte....
			//peut-être a supprimer car on s'occupe de ca dans la fonction producteur
		}
	}
	printf("Décomposition des arguments effectuee:\n	genBMP: %d\n	nbf:%d\n	nb_max_threads:%d\n 	fichier_out:%s\n",genBMP, nbf, nb_max_threads, fichier_out);
	printf("Creating fractal... \n");
	
	struct fractal *f = fractal_new("Julia", 500, 250, -0.52, 0.0);

	
	buffer = (struct fractal **) malloc(nb_max_threads * sizeof(struct fractal *));
	if (buffer == NULL)
		fprintf(stderr, "malloc error: buffer\n");
	pthread_t *readers = (pthread_t *) malloc(nbf * sizeof(pthread_t));
	if(readers == NULL)
		fprintf(stderr, "Error: assigning readers (threads) with malloc\n");
	pthread_t *computers = (pthread_t *) malloc(nb_max_threads * sizeof(pthread_t));
	if(computers == NULL)
		fprintf(stderr, "Error: assigning computers (threads) with malloc\n");

	pthread_mutex_init(&mthread_buffer, NULL);
	pthread_mutex_init(&mthread_closing, NULL);
	sem_init(&empty,0,nb_max_threads); 	//buffer vide
	sem_init(&full,0,0);				//buffer vide

	struct fractal ** buffer = (struct fractal **) malloc(nb_max_threads*sizeof(struct fractal *));
	if (buffer == NULL)
		return EXIT_FAILURE;

	for (int i = 0; i<nbf; i++){
		if ( pthread_create(&(readers[i]), NULL, &producteur, (void *) fichiers_in[i]) != 0 )
			fprintf(stderr, "Error: pthread_create readers with fichiers_in[%i]",i);
		printf("readers (threads) Created\n");
	}

	for (int i = 0; i<nb_max_threads; i++){
		int x = i;
		if ( pthread_create(&computers[i], NULL, &consommateur, (void *) x ) != 0 )
			fprintf(stderr, "Error: pthread_create computers with %i",x);
		printf("computers (threads) Created\n");
	}
/*
	for (int i = 0; i<nbf; i++)
		printf("%s\n",fractal_get_name( buffer[i]) );
*/
	/*compute_value(f);
	printf("\nConverting... \n");
	int bmp = write_bitmap_sdl(f,"julia4.bmp");
	if(bmp == 0){
		printf("\"Julia\" Converted to bmp file ! \n");
	}
	else*/
		printf("Error while while creating bmp file format\n");
	fractal_free(f);
    return 0;
}

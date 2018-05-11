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
int genBMP = 0; //indique s'il faut générer des images BMP pour toutes les fractales

double best_average;
struct fractal * best;

int nb_files_reading = 0;

int main(int argc, char *argv[]){

	int nb_max_threads = 1;

	char * fichier_out = argv[argc-1];
	char * fichiers_in[argc];


	/*Décomposition des arguments*/
	int nbf = 0;
	for (int i = 1; i < argc-1; i++){
		if (strcmp(argv[i], "-d") == 0) 		//avec l'argument -d, il faut générer des images BMP pour toutes les fractales
			genBMP = 1;
		else if (strcmp(argv[i], "--maxthreads") == 0){
			i++;
			nb_max_threads = atoi(argv[i]);
		}else{
			fichiers_in[nbf]=argv[i];
			nbf++;
		}
	}

	printf("Décomposition des arguments effectuee:\n	genBMP: %d\n	nbf:%d\n	nb_max_threads:%d\n 	fichier_out:%s\n",genBMP, nbf, nb_max_threads, fichier_out);


	nb_files_reading = nbf;
	
	// initialisation du buffer
	buffer = (struct fractal **) malloc(nb_max_threads * sizeof(struct fractal *));
	if (buffer == NULL)
		fprintf(stderr, "malloc error: buffer\n");

	//création des threads producteurs 
	pthread_t *readers = (pthread_t *) malloc(nbf * sizeof(pthread_t));
	if(readers == NULL)
		fprintf(stderr, "Error: assigning readers (threads) with malloc\n");

	//création de threads consommateurs
	pthread_t *computers = (pthread_t *) malloc(nb_max_threads * sizeof(pthread_t));
	if(computers == NULL)
		fprintf(stderr, "Error: assigning computers (threads) with malloc\n");

	pthread_mutex_init(&mthread_buffer, NULL);
	pthread_mutex_init(&mthread_closing, NULL);
	sem_init(&empty,0,nb_max_threads); 	//buffer vide
	sem_init(&full,0,0);				

	for (int i = 0; i<nbf; i++){
		if ( pthread_create(&(readers[i]), NULL, &producteur, (void *) fichiers_in[i]) != 0 )
			fprintf(stderr, "Error: pthread_create readers with fichiers_in[%i] \n",i);
	}

	for (int i = 0; i < nb_max_threads; i++){
		if ( pthread_create(&computers[i], NULL, &consommateur, NULL) != 0 )
			fprintf(stderr, "Error: pthread_create computers (%i) \n", i);
	}

	best = NULL;
	
	//on attend que tous les threads aient terminé
	for(int i = 0; i < nb_max_threads; i++){
		struct fractal * fr = fractal_new("", 0, 0, 0, 0);

		if(pthread_join(computers[i], (void ** ) &fr) != 0)
			fprintf(stderr, "pthread_join error: computers[%d]\n", i );
		
		if(best == NULL){
			best = fr;
		} else if (fractal_get_mean_value(best) < fractal_get_mean_value(fr)){
			fractal_free((struct fractal *) best);
			best = (struct fractal *) fr;
		}else {
			fractal_free(fr);
		}

	}

	/* Création d'une image BMP pour la 'meilleure' fractale */
	int bmp = write_bitmap_sdl(best, fichier_out);
	if(bmp == 0){
		printf("File: %s created\n", fichier_out);
	}
	else
		printf("Error while creating %s\n", fichier_out);
    return 0;

}

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
int genBMP = 0;

double best_average;
struct fractal * best_fractal;

int nb_files_reading = 0;

int main(int argc, char *argv[]){

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
	nb_files_reading = nbf;
	for (int i = 0; i<nbf; i++){
		if(strcmp(fichiers_in[i],"-") == 0){
			printf("Entree standard\n"); //prendre en compte....
			//peut-être a supprimer car on s'occupe de ca dans la fonction producteur
		}
	}

	printf("Décomposition des arguments effectuee:\n	genBMP: %d\n	nbf:%d\n	nb_max_threads:%d\n 	fichier_out:%s\n",genBMP, nbf, nb_max_threads, fichier_out);


	
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
	sem_init(&full,0,0);				

	struct fractal ** buffer = (struct fractal **) malloc(nb_max_threads*sizeof(struct fractal *));
	if (buffer == NULL)
		return EXIT_FAILURE;


	for (int i = 0; i<nbf; i++){
		if ( pthread_create(&(readers[i]), NULL, &producteur, (void *) fichiers_in[i]) != 0 )
			fprintf(stderr, "Error: pthread_create readers with fichiers_in[%i] \n",i);
		//printf("readers (threads) Created\n");
	}

	for (int i = 0; i<nb_max_threads; i++){
		if ( pthread_create(&computers[i], NULL, &consommateur,NULL) != 0 )
			fprintf(stderr, "Error: pthread_create computers (%i) \n", i);
		//printf("computers (threads) Created: %d\n", i);
	}

	for(int i = 0; i < nb_max_threads; i++){
		struct fractal * fr;

		if(pthread_join(computers[i], (void ** ) &fr) != 0)
			fprintf(stderr, "pthread_join error: computers[%d]\n", i );
		
		printf("Joining computers[%d]\n", i );


		if(best_fractal == NULL){
			best_fractal = fr;
			//printf("best_fractal->mean_value: %lf fr->mean_value: %lf\n", best_fractal->mean_value, fr->mean_value );	
		}
		/*else if( best_fractal -> mean_value <  fr->mean_value){
			fractal_free(best_fractal);
			best_fractal = fr;
		}
		else{
			fractal_free(fr);
		}*/

	}


	//write_bitmap_sdl(best_fractal,fichier_out);
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
	//compute_value(f);
	
		printf("END OF MAIN \n best_average: %d \n",best_average);
	//fractal_free(f);
    return 0;
}

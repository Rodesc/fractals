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

int std = 0; //False

void *producteur(void *fn){

	printf("producteur()\n");
	char * file_name = (char *) fn;
	FILE * file = NULL;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	struct fractal * fr;

	if (strcmp(file_name, "-") == 0) {
		if(std == 1){
			fprintf(stderr, "Error: invalid arguments (stdin '-' used multiple times)\n");
		}
    	file = stdin;
		std = 1;
	}
	else{
		file = fopen(file_name, "r");
	}


	if (file == NULL){
		printf("Warning: File %s doesn't exist or couldn't open. Moving on to the next file\n", file_name);
		return;	
	}

	while ((read = getline(&line, &len, file)) != -1) {
        //creer fractales a partir de line avec decodeLineToFractal()
        fr = decode_line_to_fractal(line); 
        sem_wait(&empty);
        pthread_mutex_lock(&mthread_buffer);

        //section critique
        int is_placed = 0;
        for(int i = 0; is_placed == 0; i += 1){
        	buffer[i] = fr;
        	is_placed = 1;

        }
    }
    sem_post(&full);
    pthread_mutex_unlock(&mthread_buffer);
    if(std == 0 && fclose(file) != 0){
    	fprintf(stderr, "Error: closing %s \n", file_name);
    }
}

void *consommateur(void *param){
	printf("consommateur()\n");
}

/*
*	Ebauche de fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
*	Retourne la moyenne de la fractale
*/

double compute_value(struct fractal *f){
	//struct fractal *f = (struct fractal *) fr;
	double somme = 0;
	int width = fractal_get_width(f);
	int height = fractal_get_height(f);
	printf("compute_value() started\n");
	for(int x = 0; x < width; x++ ){
		for (int y = 0; y < height; y++){
			int val = fractal_compute_value(f, x, y);
			fractal_set_value(f, x, y, val);
			fractal_set_value(f, width - x, height - y, val); 	/*par symétrie*/
			somme += 2*val; // symétrie
		}
	}
	printf("compute_value() successful\n");
	return somme/(width*height);
}


struct fractal * decode_line_to_fractal(char * l){
	char * name = (char*) malloc(sizeof(char)*64);
	printf("decode_line_to_fractal(%s)\n", l);
	if(name == NULL)
		fprintf(stderr, "Error: assigning name of fractal with malloc");
	int height; int width;
	int a; int b;
	scanf("%s %d %d %d %d", &name, &height, &width, &a, &b);
	struct fractal *f = fractal_new(name, height, width, a, b);
	free(name);
	return f;
}
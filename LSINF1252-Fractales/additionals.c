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
int nb_files_reading;
int genBMP;

int std = 0; //False
int nb_fractals = 0; //nombre de fractales dans le buffer
struct fractal * best_fractal = NULL; 

void *producteur(void *fn){

	char * file_name = (char *) fn;
	FILE * file = NULL;
	

	if (strcmp(file_name, "-") == 0) {
		if(std == 1){
			fprintf(stderr, "Error: invalid arguments (stdin '-' used multiple times)\n");
		}
		else{
			
    		file = stdin;
    		std = 1;
    		printf("Please write the description of a fractal and press enter\n");
		}
	}
	else{
		file = fopen(file_name, "r");
		if (file == NULL){
			printf("Warning: %s doesn't exist or couldn't open. Moving on to the next file\n", file_name);
			return NULL;	
		}
	}

	char line[LINE_LEN] = "";
	
   
	while (fgets(line, LINE_LEN, file) != NULL) {
        //verification si commentaire
        if( line[0] == '#'){
        	continue;
        }
        
        //transformation de la ligne en fractale
		struct fractal * fr = decode_line_to_fractal(line); 

        //section critique

        sem_wait(&empty);
    	pthread_mutex_lock(&mthread_buffer);
		
        if(buffer[nb_fractals] == NULL){
        	buffer[nb_fractals] = fr;
        	nb_fractals++;
        }
		
        sem_post(&full);
        pthread_mutex_unlock(&mthread_buffer);
    }

    if(std == 0 && fclose(file) != 0){
    	fprintf(stderr, "Error: closing %s \n", file_name);
    }
    
    pthread_mutex_lock(&mthread_closing);
    nb_files_reading--;
    pthread_mutex_unlock(&mthread_closing);
    return NULL;
}

void *consommateur(){

	struct fractal * fr = fractal_new("",0,0,0,0);

	while( nb_files_reading != 0 || nb_fractals != 0 ){
		if(nb_fractals == 0) return (void *) best_fractal;;

		sem_wait(&full);
		pthread_mutex_lock(&mthread_buffer);

		//verification
  		if (buffer[nb_fractals - 1] != NULL) {
			nb_fractals--;
  			fr = buffer[nb_fractals];
        	buffer[nb_fractals] = NULL;
  		}else{
  			fprintf(stderr, "Error with nb_fractals: %d\n", nb_fractals);
  			break;
  		}

		pthread_mutex_unlock(&mthread_buffer);
		sem_post(&empty);

		compute_value(fr); //calcul de la fractale

		//on cree tout les fichiers bmp si "-d" en arguments
		if(genBMP){
			char n[70] = "";
		    char *f_name = n;

		    strcpy(f_name, fractal_get_name(fr));
		    strcat(f_name, ".bmp");

			int bmp = write_bitmap_sdl(fr,f_name);
			if(bmp == 0){
				printf("File: %s created\n", f_name);
			}
			else
				printf("Error while creating %s\n", f_name);
		}
		
	}
	return (void*)fr;

}

/*
*	fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
*	Retourne la moyenne de la fractale
*/

double compute_value(struct fractal *f){
	//struct fractal *f = (struct fractal *) fr;
	double somme = 0;
	int height = fractal_get_height(f);
	int width = fractal_get_width(f);
	for(int x = 0; x < width; x++ ){
		for (int y = 0; y < height; y++){
			int val = fractal_compute_value(f, x, y);
			fractal_set_value(f, x, y, val);
			somme = somme + val; 
		}
	}
	double moyenne = somme/(width*height);
	f->mean_value = moyenne;
	return moyenne;
}

/* 
	Converti un ligne l en fractale
 */
struct fractal * decode_line_to_fractal(char * l){

	char * name = (char*) malloc(sizeof(char)*64);
	if(name == NULL)
		fprintf(stderr, "Error: assigning name of fractal with malloc");

	int height; int width;
	double a; double b;
	sscanf(l, "%s %d %d %lf %lf", name, &height, &width, &a, &b);
	struct fractal *f = fractal_new(name, height, width, a, b);
	//free(name);
	return f;
}

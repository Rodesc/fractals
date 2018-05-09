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
int nb_fractals = 0;
double best_average = 0;
struct fractal * best_fractal = NULL;
void *producteur(void *fn){

	char * file_name = (char *) fn;
	FILE * file = NULL;
	char line[LINE_LEN] = "";
	ssize_t read;
	struct fractal * fr;

	if (strcmp(file_name, "-") == 0) {
		if(std == 1){
			fprintf(stderr, "Error: invalid arguments (stdin '-' used multiple times)\n");
			return;
		}
		printf("Waiting for stdin..\n");
    	file = stdin;
		std = 1;
	}
	else{
		file = fopen(file_name, "r");
	}
	printf("Opening file: %s\n", file_name);
	if (file == NULL){
		printf("Warning: File %s doesn't exist or couldn't open. Moving on to the next file\n", file_name);
		return;	
	}

	while (1) {
		if(fgets(line, LINE_LEN, file) == NULL) break;
        //creer fractales a partir de line avec decodeLineToFractal()
        if( line[0] == '#'){
        	printf("Line is comment: \"%s\"\n", line);
        	continue;
        }

        printf("decode_line_to_fractal( %s )\n", file_name);
        fr = decode_line_to_fractal(line); 
        printf("Line decoded to fractal: %s\n", fractal_get_name(fr));
        sem_wait(&empty);
        pthread_mutex_lock(&mthread_buffer);
		
        //section critique
        /*int is_placed = 0;
        for(int i = 0; is_placed == 0; i += 1){
        	buffer[i] = fr;
        	is_placed = 1;
        	nb_fractals ++;

        }*/

        if(buffer[nb_fractals] == NULL){
        	buffer[nb_fractals] = fr;
        	nb_fractals++;
        }

        sem_post(&full);
	    pthread_mutex_unlock(&mthread_buffer);
    }

    //code de vérification
    for(int i = 0; i < nb_fractals; i++){
    	printf("\t buffer[%d] -> %s\n", i, fractal_get_name(buffer[i]) );
    }
    printf("Closing file \"%s\"\n", file_name );
    if(std == 0 && fclose(file) != 0){
    	fprintf(stderr, "Error: closing %s \n", file_name);
    }
    nb_files_reading--;

    pthread_mutex_lock(&mthread_closing);
    pthread_mutex_unlock(&mthread_closing);
}

void *consommateur(void *param){
	printf("nb_files_reading: %d\n", nb_files_reading );

	struct fractal * fr = NULL;
	while( nb_files_reading != 0){
		sem_wait(&full);
		pthread_mutex_unlock(&mthread_buffer);
  		if (buffer[nb_fractals] != NULL) {
  			fr = buffer[nb_fractals];
			nb_fractals--;
        	buffer[nb_fractals] = NULL;
  		}else{
  			fprintf(stderr, "Error with nb_fractals\n");
  			break;
  		}
		
		pthread_mutex_unlock(&mthread_buffer);
		sem_post(&empty);

		
		if(1){
			printf("print it!\n");
			printf("Name of fractal to print: %s\n",fractal_get_name(fr) );
			char * fname = strcat(fractal_get_name(fr), ".bmp");
			printf("lilsissterrrrrrr\n");
			write_bitmap_sdl(fr,fname);
			printf("brooo\n");
		}
		/*
		double fr_average = compute_value(fr);
		if( fr_average >= best_average ){
			fractal_free(best_fractal);
			best_fractal = fr;
		}else{
			fractal_free(fr);
		}

		return (void *) best_fractal;
		*/
	}

}

/*
*	fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
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

/* 
	Converti un ligne l en fractale
 */
struct fractal * decode_line_to_fractal(char * l){
	printf("decode_line_to_fractal(%s)\n", l);

	char * name = (char*) malloc(sizeof(char)*64);
	if(name == NULL)
		fprintf(stderr, "Error: assigning name of fractal with malloc");

	int height; int width;
	double a; double b;
	sscanf(l, "%s %d %d %lf %lf", name, &height, &width, &a, &b);
	struct fractal *f = fractal_new(name, height, width, a, b);
	free(name);
	return f;
}

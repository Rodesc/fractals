#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "libfractal/fractal.h"

int main(int argc, char *argv[]){

	/*VERSION 2*/
	int genBMP;
	/*Décomposition des arguments*/
	if(argc >= 2 && strcmp(argv[1], "-d") == 0){
		genBMP = 1;
		printf("*** Generate .bmp files \n");
	}

	printf("Creating fractal... \n");
	
	struct fractal *f = fractal_new("Julia", 100, 250, 0.3, 0.8);
	printf("OK \nCreating compute_thread ...\n");
	
	pthread_t compute_thread;
	printf("OK \nComputing \"Julia\"... \n");
	
	/*if(pthread_create(&compute_thread, NULL, compute_value(), f)){
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}*/
	
	/*pthread_create(&compute_thread, NULL, compute_value(f), NULL);
	
	/*pthread_join(compute_thread, NULL);*/
	/*if(pthread_join(compute_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;

	}*/
	compute_value(f);
	printf("OK \nConverting... \n");
	int bmp = write_bitmap_sdl(f,"julia4.bmp");
	if(bmp == 0){
		printf("\"Julia\" Converted to bmp file ! \n");
		system("ristretto julia4.bmp");
	}
	else
		printf("Error while while creating bmp file format\n");
	fractal_free(f);
    return 0;
}

/*
*	Ebauche de fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
*/

void compute_value(struct fractal *f){
	printf("compute_value() started\n");
	for(int i = 0; i < f->width; i++ ){
		for (int j = 0; j < f->height; j++){
			int val = fractal_compute_value(f, i, j);
			fractal_set_value(f, i, j, val);
			/*fractal_set_value(f, f->width - i, f->height - j, val); 	/*par symétrie*/
		}
	}
	printf("compute_value() finished\n");
	return NULL;
}
struct fractal * decode_line_to_fractal(char * l){
	char * name;
	int height; int width;
	int a; int b;
	scanf("%s %d %d %d %d", &name, &height, &width, &a, &b);
	struct fractal *f = fractal_new(name, height, width, a, b);
	return f;
}
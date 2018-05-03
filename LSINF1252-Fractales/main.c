#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "libfractal/fractal.h"
/*
*	Ebauche de fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
*	Retourne la moyenne de la fractale
*/

double compute_value(struct fractal *f){
	double moyenne = 0;
	int width = fractal_get_width(f);
	int height = fractal_get_height(f);
	printf("compute_value() started\n");
	for(int x = 0; x < width; x++ ){
		for (int y = 0; y < height; y++){
			int val = fractal_compute_value(f, x, y);
			fractal_set_value(f, x, y, val);
			fractal_set_value(f, width - x, height - y, val); 	/*par symétrie*/
			moyenne += 2*val; // symétrie
		}
	}
	return moyenne/(width*height);
}
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
		else if (strcmp(argv[i], "--maxthread") == 0){
			i++;
			nb_max_threads = atoi(argv[i]);
		}else{
			fichiers_in[nbf]=argv[i];
			nbf++;
		}
	}
	for (int i = 0; i<nbf; i++){
		printf("%s\n",fichiers_in[i]);
		if(strcmp(fichiers_in[i],"-") == 0){
			printf("Entree standard\n"); //prendre en compte....
		}
	}
	printf("Décomposition des arguments effectuee:\n	genBMP: %d\n	nbf:%d\n	nb_max_threads:%d\n",genBMP, nbf, nb_max_threads);
	printf("Creating fractal... \n");
	
	struct fractal *f = fractal_new("Julia", 500, 250, 0.6, 0.8);
	pthread_t compute_thread;
	/*if(pthread_create(&compute_thread, NULL, compute_value(), f)){
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}*/
	
	/*pthread_create(&compute_thread, NULL, compute_value(f), NULL);
	
	pthread_join(compute_thread, NULL);
	if(pthread_join(compute_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;

	}*/
	compute_value(f);
	printf("\nConverting... \n");
	int bmp = write_bitmap_sdl(f,"julia4.bmp");
	if(bmp == 0){
		printf("\"Julia\" Converted to bmp file ! \n");
	}
	else
		printf("Error while while creating bmp file format\n");
	fractal_free(f);
    return 0;
}


struct fractal * decode_line_to_fractal(char * l){
	char * name = (char*) malloc(sizeof(char)*64);
	if(name == NULL)
		printf("malloc error");
	int height; int width;
	int a; int b;
	scanf("%s %d %d %d %d", &name, &height, &width, &a, &b);
	struct fractal *f = fractal_new(name, height, width, a, b);
	free(name);
	return f;
}
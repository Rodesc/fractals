#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "libfractal/fractal.h"

int main(){

	/*VERSION 1*/

	printf("Creating fractal... \n");
	struct fractal *f = fractal_new("Julia", 192, 108, 0.3, 0.5);
	printf("Fractal \"Julia\" created... \nComputing \"Julia\"... \n");
	compute_value(f);
	printf("Computed \"Julia\"... \nConverting... \n");
	int bmp = write_bitmap_sdl(f,"Julia1.bmp");
	if(bmp == 0){
		printf("\"Julia\" Converted to bmp file ! \n");
		system("ristretto Julia1.bmp");
	}
	else
		printf("Error while while creating bmp file format\n");
    return 0;
}

/*
*	Ebauche de fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
*/

void compute_value(struct fractal *f){

	for(int i = 0; i < f->width; i++ ){
		for (int j = 0; j <= f->height/2; j++){
			int val = fractal_compute_value(f, i, j);
			fractal_set_value(f, i, j, val);
			fractal_set_value(f, f->width - i, f->height - j, val); 	/*par symétrie*/
		}
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "fractal.h"

int main(){

    /* TODO */
	printf("Compilation test succesfull\n", );
    return 0;
}

/*
*	Ebauche de fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
*/
void compute_value(struct fractal *f){

	for(int i = 0, i < f->(width/2), i++ ){
		for (int j = 0, j < f->(height/2), j++){
			int val = fractal_compute_value(*f, i, j);
			fractal_set_value(*f, i, j, val);
			fractal_set_value(*f, f->width - i, f->height - j, val); 	/*par symétrie*/
		}
	}
}

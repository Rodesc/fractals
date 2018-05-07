#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "libfractal/fractal.h"
#include "additionals.h"

void *producteur(void *fn){

	printf("producteur()");

	char * file_name = (char *) fn;
	FILE * file = NULL;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int std = 0; //False

	if (strcmp(file_name, "-") == 0) {
    	file = stdin;
		std = 1;
	}
	else{
		file = fopen(file_name, "r");
	}


	if (file == NULL){
		printf("Warning: File %s doesn't exist or couldn't open. Moving on to the next file", file_name);
		return;	
	}

	while ((read = getline(&line, &len, file)) != -1) {
        //creer fractales a partir de line avec decodeLineToFractal() 
    }

}

void *consommateur(void *param){
	printf("consommateur()");
}
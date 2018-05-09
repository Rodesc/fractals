#ifndef _MAIN_H
#define _MAIN_H

extern struct fractal ** buffer;

//Déclarations des mutex et semaphores
extern pthread_mutex_t mthread_buffer;
extern pthread_mutex_t mthread_closing;
extern sem_t empty;
extern sem_t full;

#endif
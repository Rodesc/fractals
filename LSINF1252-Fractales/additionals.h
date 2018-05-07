#ifndef _ADDITIONALS_H
#define _ADDITIONALS_H

/*
 * read_file lit un fichier et cree des fractales.
 * @return: 0 si on a atteint la fin du fichier sans erreur
 *          1 si succes
 *
 * 1 mutex par ligne lue
 */
extern void *producteur(void *fn);

/*
 * calculator description
 * @fract:
 * @return:
 */
extern void *consommateur(void * bfi);

#endif
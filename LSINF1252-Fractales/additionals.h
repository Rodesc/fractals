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


/*
*	Ebauche de fonction qui calcule la fractale en prenant en compte la propriété de symétrie des fractales de Julia. 
*	Retourne la moyenne de la fractale
*/
extern double compute_value(struct fractal *f);

struct fractal * decode_line_to_fractal(char * l);
#endif
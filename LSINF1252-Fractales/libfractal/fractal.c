#include <stdlib.h>
#include <string.h>
#include "fractal.h"

struct fractal *fractal_new(const char *name, int width, int height, double a, double b)
{
	
	struct fractal *f = malloc(1, sizeof *f + sizeof(double[height][width]) ); // using calloc since it conveniently fills everything with zeroes
	/*f->name = name;
	*/
	strcpy(f->name, name );
	f->height = height;
	f->width = width;
	f->a = a;
	f->b = b;
	return f;
}

void fractal_free(struct fractal *f)
{
    /* TODO */
    free(f);
}

const char *fractal_get_name(const struct fractal *f)
{
	/* TODO */
	if (f == NULL)
		return NULL;
    return f->name;
}

int fractal_get_value(const struct fractal *f, int x, int y)
{
    /* TODO */
    double (*array_2D)[f->width] = (double(*)[f->width]) f->values;
    return array_2D[y][x];
}

void fractal_set_value(struct fractal *f, int x, int y, int val)
{
    /* TODO */
    double (*array_2D)[f->width] = (double(*)[f->width]) f->values;
    array_2D[y][x] = val; 
}

int fractal_get_width(const struct fractal *f)
{
    /* TODO */
    return f->width;
}

int fractal_get_height(const struct fractal *f)
{
    /* TODO */
    return f->height;
}

double fractal_get_a(const struct fractal *f)
{
    /* TODO */
    return f->a;
}

double fractal_get_b(const struct fractal *f)
{
    /* TODO */
    return f->b;
}

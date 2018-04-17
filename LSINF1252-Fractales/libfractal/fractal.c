#include <stdlib.h>
#include "fractal.h"

struct fractal *fractal_new(const char *name, int width, int height, double a, double b)
{
    /* TODO */
    struct fractal f;
    f.name = name;
    f.height = height;
    f.width = width;
    f.a = a;  
    f.b = b;
    struct fractal *ptr = f;
    return ptr;
}

void fractal_free(struct fractal *f)
{
    /* TODO */
    *f = NULL;
}

const char *fractal_get_name(const struct fractal *f)
{
    /* TODO */
    return f->name;
}

int fractal_get_value(const struct fractal *f, int x, int y)
{
    /* TODO */
    return f->value[x][y];
}

void fractal_set_value(struct fractal *f, int x, int y, int val)
{
    /* TODO */
    f->values[x][y] = val;
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

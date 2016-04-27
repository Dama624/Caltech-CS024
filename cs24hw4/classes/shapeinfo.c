#include <stdio.h>
#include <stdlib.h>

#include "shapes.h"


/* This helper function prints some useful information about any Shape that is
 * passed to it.
 */
void print_info(const char *type, Shape_Data *s) {
    float mass, volume;
    
    mass = Shape_getMass(s);
    volume = s->class->getVolume(s);

    printf("Volume of %s:  %f\tMass of %s:  %f\n\n", type, volume, type, mass);
}


/* A test program for the shape classes! */
int main() {
    Cone_Data *c;
    Box_Data *b;
    Sphere_Data *s;

    /* Initialize the class information for all classes. */
    static_init();

    printf("\n");

    c = new_Cone(1.5, 5, 0.5);
    print_info("cone", (Shape_Data *) c);
    free(c);

    b = new_Box(5.3, 2.1, 7.7, 3);
    print_info("box", (Shape_Data *) b);
    free(b);

    s = new_Sphere(0.3, 10);
    print_info("sphere", (Shape_Data *) s);
    free(s);

    return 0;
}


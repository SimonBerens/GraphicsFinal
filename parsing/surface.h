#ifndef SURFACE_H
#define SURFACE_H

#include "../scalables/scalable.h"

struct Surface {
    Surface(const Scalable<double, 3> &ambient,
            const Scalable<double, 3> &diffuse,
            const Scalable<double, 3> &specular);

    Scalable<double, 3> ambient;
    Scalable<double, 3> diffuse;
    Scalable<double, 3> specular;
};


#endif //SURFACE_H

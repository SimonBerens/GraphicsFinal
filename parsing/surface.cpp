#include "surface.h"

Surface::Surface(const Scalable<double, 3> &ambient,
                 const Scalable<double, 3> &diffuse,
                 const Scalable<double, 3> &specular) :
                 ambient(ambient), diffuse(diffuse), specular(specular) {}

#ifndef SYM_H
#define SYM_H


#include <variant>
#include <memory>
#include "../matrices/m_matri.h"
#include "../scalables/scalable.h"
#include "../scalables/P.h"
#include "../scalables/color.h"


struct Surface {
    Surface(const Scalable<double, 3> &ambient, const Scalable<double, 3> &diffuse,
            const Scalable<double, 3> &specular);

    Scalable<double, 3> ambient;
    Scalable<double, 3> diffuse;
    Scalable<double, 3> specular;
};

struct Light {
    Light(const Color &color, const P &location);

    Color color;
    P location;
};

#endif //SYM_H

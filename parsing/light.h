#ifndef LIGHT_H
#define LIGHT_H


#include <variant>
#include <memory>
#include "../matrices/modifier_matrix.h"
#include "../scalables/scalable.h"
#include "../scalables/point.h"
#include "../scalables/color.h"



struct Light {
    Light(const Color &color, const Point &location);

    Color color;
    Point location;
};

#endif //LIGHT_H

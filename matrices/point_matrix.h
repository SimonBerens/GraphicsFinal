#ifndef POINT_MATRIX_H
#define POINT_MATRIX_H

#include <vector>
#include "../scalables/point.h"
#include "modifier_matrix.h"

class PointMatrix {
public:

    PointMatrix();

    typedef unsigned int size_type;

    size_type cols() const;

    Point operator[](size_type col) const;

    Point &operator[](size_type col);

    virtual PointMatrix &mult(const ModifierMatrix &modifier);

    friend std::ostream &operator<<(std::ostream &os, const PointMatrix &matrix);

protected:
    void add_point(const Point &p);

private:
    std::vector<Point> data;

    double operator()(size_type row, size_type col) const;

    double &operator()(size_type row, size_type col);

};

#endif //POINT_MATRIX_H

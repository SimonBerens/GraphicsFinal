#ifndef MODIFIER_MATRIX_H
#define MODIFIER_MATRIX_H

#include <array>
#include <ostream>

class ModifierMatrix {
public:
    typedef unsigned int size_type;

    explicit ModifierMatrix(bool identity = true);

    void mult(const ModifierMatrix &other);

    friend std::ostream &operator<<(std::ostream &os, const ModifierMatrix &matrix);

    friend class PointMatrix;

private:
    std::array<std::array<double, 4>, 4> data{};


protected:
    double operator()(size_type row, size_type col) const;

    double &operator()(size_type row, size_type col);

};


#endif //MODIFIER_MATRIX_H

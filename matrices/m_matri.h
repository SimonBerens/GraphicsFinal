#ifndef M_MATRI_H
#define M_MATRI_H

#include <array>
#include <ostream>

class M_Matrix {
public:
    typedef unsigned int size_type;

    explicit M_Matrix(bool identity = true);

    void mult(const M_Matrix &other);

    friend std::ostream &operator<<(std::ostream &os, const M_Matrix &matrix);

    friend class P_Matrix;

private:
    std::array<std::array<double, 4>, 4> data{};


protected:
    double operator()(size_type row, size_type col) const;

    double &operator()(size_type row, size_type col);

};


#endif //M_MATRI_H

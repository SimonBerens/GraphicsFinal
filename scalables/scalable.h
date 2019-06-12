#ifndef SCALABLE_H
#define SCALABLE_H

#include <array>
#include <functional>
#include <ostream>

template<typename T, int size>
class Scalable {
public:

    Scalable() = default;

    Scalable(T scalar) {
        for (auto &val : data)
            val = scalar;
    }

    Scalable(std::array<T, size> values) : data(values) {}

    T &operator[](unsigned int n) {
        return data[n];
    }

    T operator[](unsigned int n) const {
        return data[n];
    }

    friend Scalable operator+(const Scalable &a, const Scalable &b) {
        Scalable<T, size> result;
        for (int i = 0; i < size; ++i)
            result.data[i] = a.data[i] + b.data[i];
        return result;
    };

    friend Scalable operator-(const Scalable &a, const Scalable &b) {
        Scalable<T, size> result;
        for (int i = 0; i < size; ++i)
            result.data[i] = a.data[i] - b.data[i];
        return result;
    }

    friend Scalable operator*(const Scalable &a, const Scalable &b) {
        Scalable<T, size> result;
        for (int i = 0; i < size; ++i)
            result.data[i] = a.data[i] * b.data[i];
        return result;
    }

    friend Scalable operator/(const Scalable &a, const Scalable &b) {
        Scalable<T, size> result;
        for (int i = 0; i < size; ++i)
            result.data[i] = a.data[i] / b.data[i];
        return result;
    }

    friend std::ostream &operator<<(std::ostream &os, const Scalable &scalable) {
        for (auto &val : scalable.data)
            os << val << " ";
        return os;
    }

protected:
    std::array<T, size> data;
};

#endif //SCALABLE_H

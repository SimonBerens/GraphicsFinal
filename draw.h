#ifndef DRAW_H
#define DRAW_H

#include "custom_matrices.h"
#include "math_vec.h"
#include "color.h"
#include "symbol_table.h"

class Frame {
    const int width;
    const int height;
    Color **grid;
    double **zbuf;
public:
    Frame(int width, int height);

    ~Frame();

    const int getWidth() const;

    const int getHeight() const;

    void clear();

    void write_to(FILE *f);

    void save(const std::string &name);

    void plot(int x, int y, double z, const Color &c);

    void draw_line(double x0, double y0, double z0, double x1, double y1, double z1, const Color &c);

    void draw_line(const Point &p0, const Point &p1, const Color &c);

    void draw_lines(const EdgeList &el);

    static Vec_3D surface_normal(const Point &p0, const Point &p1, const Point &p2);

    void draw_faces(const TriangleMatrix &tm, constants *lighting);

    static Color calculate_color(const Point &p0, const Point &p1, const Point &p2, constants *lighting);

    void draw_scanline(int x0, double z0, int x1, double z1, int y, const Color &c);

    void fill_triangle(const Point &p0, const Point &p1, const Point &p2, constants *lighting);

};

#endif //CURVES_DRAW_H

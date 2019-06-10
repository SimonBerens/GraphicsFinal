#ifndef DRAW_H
#define DRAW_H

#include "scalables/color.h"
#include "symbol_table.h"
#include "scalables/P.h"
#include "scalables/V.h"
#include "matrices/EdgeList.h"
#include "matrices/face.h"

class Frame {
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

    void draw_line(const P &p0, const P &p1, const Color &c);

    void draw_lines(const EL &el);

    static V surface_normal(const P &p0, const P &p1, const P &p2);

    void draw_faces(const FL &tm, constants *lighting);

    static Color calculate_color(const P &p0, const P &p1, const P &p2, constants *lighting);

    void draw_scanline(int x0, double z0, int x1, double z1, int y, const Color &c);

    void fill_triangle(const P &p0, const P &p1, const P &p2, constants *lighting);

private:
    const int width;
    const int height;
    Color **grid;
    double **zbuf;
};

#endif //DRAW_H

#include <algorithm>
#include <cfloat>
#include <random>
#include <iostream>
#include "draw.h"
#include "symbol_table.h"
#include "scalables/V.h"

using namespace std;

#define DEFAULT_COLOR Color(28, 237, 66)

Frame::Frame(const int width, const int height) : width(width), height(height) { // todo array this shit
    this->grid = new Color *[height];
    zbuf = new double *[height];
    for (int row = 0; row < height; ++row) {
        this->grid[row] = new Color[width];
        this->zbuf[row] = new double[width];
        fill(this->zbuf[row], this->zbuf[row] + width, -DBL_MAX);
    }
}

Frame::~Frame() {
    for (int row = 0; row < this->height; ++row) {
        delete[] this->grid[row];
        delete[] this->zbuf[row];
    }
    delete[] this->grid;
    delete[] this->zbuf;
}

const int Frame::getWidth() const {
    return width;
}

const int Frame::getHeight() const {
    return height;
}

void Frame::clear() {
    for (int row = 0; row < this->height; ++row) {
        delete[] this->grid[row];
        delete[] this->zbuf[row];

    }
    delete[] this->grid;
    delete[] this->zbuf;
    this->grid = new Color *[height];
    this->zbuf = new double *[height];
    for (int row = 0; row < height; ++row) {
        this->grid[row] = new Color[width];
        this->zbuf[row] = new double[width];
        fill(this->zbuf[row], this->zbuf[row] + width, -DBL_MAX);
    }
}

void Frame::write_to(FILE *f) {
    fprintf(f, "P3\n%d %d\n%d\n", getWidth(), getHeight(), 255);
    Color *c;
    for (int y = getHeight() - 1; y >= 0; y--) {
        for (int x = 0; x < getWidth(); x++) {
            c = &grid[y][x];
            fprintf(f, "%d %d %d ", Color::bound(c->red()), Color::bound(c->green()), Color::bound(c->blue()));
        }
        fprintf(f, "\n");
    }
}

void Frame::save(const string &name) {
    FILE *fd = popen(("convert - " + name).c_str(), "w");
    write_to(fd);
    fclose(fd);
}

inline void Frame::plot(int x, int y, double z, const Color &c) {
    if (x >= 0 && x < width && y >= 0 && y < height && zbuf[y][x] < z - 0.0001) {
        grid[y][x] = c;
        zbuf[y][x] = z;
    }
}

void Frame::draw_line(double x0d, double y0d, double z0, double x1d, double y1d, double z1, const Color &c) {
    int x0 = round(x0d), y0 = round(y0d), x1 = round(x1d), y1 = round(y1d);
    int x, y, d, A, B;
    double z, dz;
    int dy_east, dy_northeast, dx_east, dx_northeast, d_east, d_northeast;
    int loop_start, loop_end;

    //swap points if going right -> left
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
        swap(z0, z1);
    }

    x = x0;
    y = y0;
    z = z0;
    A = 2 * (y1 - y0);
    B = -2 * (x1 - x0);
    int wide = 0;
    int tall = 0;
    //octants 1 and 8
    if (abs(x1 - x0) >= abs(y1 - y0)) { //octant 1/8
        wide = 1;
        loop_start = x;
        loop_end = x1;
        dx_east = dx_northeast = 1;
        dy_east = 0;
        d_east = A;
        if (A > 0) { //octant 1
            d = A + B / 2;
            dy_northeast = 1;
            d_northeast = A + B;
        } else { //octant 8
            d = A - B / 2;
            dy_northeast = -1;
            d_northeast = A - B;
        }
    }//end octant 1/8
    else { //octant 2/7
        tall = 1;
        dx_east = 0;
        dx_northeast = 1;
        if (A > 0) {     //octant 2
            d = A / 2 + B;
            dy_east = dy_northeast = 1;
            d_northeast = A + B;
            d_east = B;
            loop_start = y;
            loop_end = y1;
        } else {     //octant 7
            d = A / 2 - B;
            dy_east = dy_northeast = -1;
            d_northeast = A - B;
            d_east = -1 * B;
            loop_start = y1;
            loop_end = y;
        }
    }
    dz = loop_end == loop_start ? 0 : (z1 - z0) / (loop_end - loop_start);


    while (loop_start < loop_end) {

        plot(x, y, z, c);
        if ((wide && ((A > 0 && d > 0) ||
                      (A < 0 && d < 0)))
            ||
            (tall && ((A > 0 && d < 0) ||
                      (A < 0 && d > 0)))) {
            y += dy_northeast;
            d += d_northeast;
            x += dx_northeast;
        } else {
            x += dx_east;
            y += dy_east;
            d += d_east;
        }
        z += dz;
        loop_start++;
    } //end drawing loop
    plot(x, y, z, c);
}

void Frame::draw_line(const P &p0, const P &p1, const Color &c) {
    draw_line(p0.x(), p0.y(), p0.z(), p1.x(), p1.y(), p1.z(), c);
}

void Frame::draw_lines(const EL &el) {
    for (int i = 0; i < el.cols(); i += 2)
        draw_line(el[i], el[i + 1], DEFAULT_COLOR);
}

V Frame::surface_normal(const P &p0, const P &p1, const P &p2) {
    V a = P(p1 - p0), b = P(p2 - p0);
    return cross(a, b);
}


void Frame::draw_faces(const FL &tm, constants *lighting) { // todo names
    for (int i = 0; i < tm.cols(); i += 3) {
        V normal = surface_normal(tm[i], tm[i + 1], tm[i + 2]);
        V view(0, 0, 1);
        if (dot(view, normal) > 0)
            fill_triangle(tm[i], tm[i + 1], tm[i + 2], lighting);
    }
}

Color Frame::calculate_color(const P &p0, const P &p1, const P &p2, constants *lighting) {
    // not yet implemented
    static Color ambient_light(50, 50, 50), point_light_c(255, 255, 255);
    static P point_light_p(1000, 600, 1000);
    V view{0, 0, 1};

    // defaults
    Scalable<double, 3> ka(0.5), kd(0.5), ks(0.5);
    if (lighting) {
        ka = {{lighting->r[0], lighting->g[0], lighting->b[0]}};
        kd = {{lighting->r[1], lighting->g[1], lighting->b[1]}};
        ks = {{lighting->r[2], lighting->g[2], lighting->b[2]}};
    }
    // calculation
    int exp = 3;
    V normal = surface_normal(p0, p1, p2);
    V l = P(point_light_p - p0);
    Color ambient = ka * ambient_light;
    double lnd = dot(l.normalized(), normal.normalized());
    Color diffuse(lnd * kd * point_light_c);
    Color specular(pow(dot(view, 2 * lnd * normal.normalized() - l.normalized()), exp) * ks * point_light_c);
    return {ambient + diffuse + specular};
}

void Frame::draw_scanline(int x0, double z0, int x1, double z1, int y, const Color &c) {
    //swap if needed to assure left->right drawing
    if (x0 > x1) {
        swap(x0, x1);
        swap(z0, z1);
    }

    double delta_z;
    delta_z = (x1 - x0) != 0 ? (z1 - z0) / (x1 - x0 + 1) : 0;
    int x;
    double z = z0;

    for (x = x0; x <= x1; x++) {
        plot(x, y, z, c);
        z += delta_z;
    }
}

void Frame::fill_triangle(const P &p0, const P &p1, const P &p2, constants *lighting) {
    Color c = calculate_color(p0, p1, p2, lighting);

    auto ycomp = [](const P &p0, const P &p1) -> bool { return p0.y() == p1.y() ? p0.x() < p1.x() : p0.y() < p1.y(); };
    vector<P> pts{p0, p1, p2};
    sort(pts.begin(), pts.end(), ycomp);
    int yb = pts[0].y(), ym = pts[1].y(), yt = pts[2].y();
    double xb = pts[0].x(), xm = pts[1].x(), xt = pts[2].x(),
            zb = pts[0].z(), zm = pts[1].z(), zt = pts[2].z();

    int y = yb;
    double x0 = xb, x1 = xb, z0 = zb, z1 = zb;
    double dx0 = (xt - xb) / (yt - yb), dz0 = (zt - zb) / (yt - yb);
    if (ym != yb) {
        double dx1 = (xm - xb) / (ym - yb), dz1 = (zm - zb) / (ym - yb);

        for (; y < ym; y++) {
            draw_scanline(x0, z0, x1, z1, y, c);
            x0 += dx0, x1 += dx1, z0 += dz0, z1 += dz1;
        }
    }
    if (yt != ym) {
        x1 = xm, z1 = zm;
        double dx1 = (xt - xm) / (yt - ym), dz1 = (zt - zm) / (yt - ym);
        for (; y < yt; y++) {
            draw_scanline(x0, z0, x1, z1, y, c);
            x0 += dx0, x1 += dx1, z0 += dz0, z1 += dz1;
        }
    }

}
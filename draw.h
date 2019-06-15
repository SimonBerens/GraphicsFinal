#ifndef DRAW_H
#define DRAW_H

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <array>
#include <cmath>
#include <cfloat>
#include "scalables/color.h"
#include "scalables/point.h"
#include "scalables/vec.h"
#include "matrices/face_list.h"
#include "parsing/light.h"
#include "parsing/surface.h"

template<int width, int height>
class Frame {
public:
    Frame() {
        for (auto &row: zbuf)
            row.fill(-DBL_MAX);
    }

    void write_to(FILE *f) {
        fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
        for (int y = height - 1; y >= 0; y--) {
            for (int x = 0; x < width; x++) {
                Color &c = grid[y][x];
                fprintf(f, "%d %d %d ",
                        static_cast<int>(c.red()), static_cast<int>(c.green()), static_cast<int>(c.blue()));
            }
            fprintf(f, "\n");
        }
    }

    void save(const std::string &name) {
        FILE *fd = popen(("convert - " + name).c_str(), "w");
        write_to(fd);
        fclose(fd);
    }

    void plot(int x, int y, double z, const Color &c) {
        if (x >= 0 && x < width && y >= 0 && y < height && zbuf[y][x] < z - 0.0001) {
            grid[y][x] = c;
            zbuf[y][x] = z;
        }
    }

    void draw_line(double x0d, double y0d, double z0, double x1d, double y1d, double z1, const Color &c) {
        int x0 = round(x0d), y0 = round(y0d), x1 = round(x1d), y1 = round(y1d);
        int x, y, d, A, B;
        double z, dz;
        int dy_east, dy_northeast, dx_east, dx_northeast, d_east, d_northeast;
        int loop_start, loop_end;

        //swap points if going right -> left
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
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


    void draw_line(const Point &p0, const Point &p1, const Color &c) {
        draw_line(p0.x(), p0.y(), p0.z(), p1.x(), p1.y(), p1.z(), c);
    }

    static Vec3D surface_normal(const Point &p0, const Point &p1, const Point &p2) {
        Vec3D a = Point(p1 - p0), b = Point(p2 - p0);
        return cross(a, b);
    }

    void draw_faces(const FaceList &fl, const Surface &surface, const Color & ambient_light, const std::vector<Light> &lights) {
        for (int i = 0; i < fl.cols(); i += 3) {
            Vec3D normal = surface_normal(fl[i], fl[i + 1], fl[i + 2]);
            Vec3D view(0, 0, 1);
            if (dot(view, normal) > 0)
                fill_triangle(fl[i], fl[i + 1], fl[i + 2], surface, ambient_light, lights);
        }
    }

    static Color
    calculate_color(const Point &p0, const Point &p1, const Point &p2, const Surface &surface,
                    const Color &ambient_light, const std::vector<Light> &lights) {
        // not yet implemented
        Vec3D view{0, 0, 1};
        // calculation
        int exp = 3;
        Vec3D normal = surface_normal(p0, p1, p2);
        Color ret;
        Color ambient = surface.ambient * ambient_light;
        for (auto &light : lights) {
            Vec3D l = Point(light.location - p0);
            double lnd = dot(l.normalized(), normal.normalized());
            Color diffuse(lnd * surface.diffuse * light.color);
            Color specular(
                    pow(dot(view, 2 * lnd * normal.normalized() - l.normalized()), exp) * surface.specular *
                    light.color);
            ret = ret + diffuse.bound() + specular.bound();
        }
        ret = ret + ambient.bound();
        return ret.bound();
    }

    void draw_scanline(int x0, double z0, int x1, double z1, int y, const Color &c) {
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(z0, z1);
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


    void fill_triangle(const Point &p0, const Point &p1, const Point &p2, const Surface &surface,
                       const Color &ambient_light, const std::vector<Light> &lights) {
        Color c = calculate_color(p0, p1, p2, surface, ambient_light, lights);

        auto ycomp = [](const Point &p0, const Point &p1) -> bool {
            return p0.y() == p1.y() ? p0.x() < p1.x() : p0.y() < p1.y();
        };
        std::array<Point, 3> pts{p0, p1, p2};
        std::sort(pts.begin(), pts.end(), ycomp);
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

private:
    std::array<std::array<Color, height>, width> grid;
    std::array<std::array<double, height>, width> zbuf;
};

#endif //DRAW_H

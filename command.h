#ifndef PARSER_H
#define PARSER_H

#include "symbol_table.h"

#define MAX_COMMANDS 512

extern int lastop;

#define Ka 0
#define Kd 1
#define Ks 2


struct command
{
    int opcode;
    union
    {
        struct {
            SYMBOL *p;
            double c[4];
        } light;
        struct {
            double c[4];
        } ambient;
        struct {
            SYMBOL *p;
            /* each triple holds ka kd and ks for red green and blue
               respectively */
        } constants;
        struct {
            SYMBOL *p;
        } save_coordinate_system;
        struct {
            double eye[4],aim[4];
        } camera;
        struct {
            SYMBOL *constants;
            double d[4];
            double r;
            SYMBOL *cs;
        } sphere;
        struct {
            SYMBOL *constants;
            double d0[3];
            double d1[3];
            double d2[3];
            double d3[3];
            SYMBOL *p;
            SYMBOL *cs;
        } texture;
        struct {
            SYMBOL *constants;
            double d[4];
            double r0,r1;
            SYMBOL *cs;
        } torus;
        struct {
            SYMBOL *constants;
            double d0[4],d1[4];
            SYMBOL *cs;
        } box;
        struct {
            SYMBOL *constants;
            double p0[4],p1[4];
            SYMBOL *cs0,*cs1;
        } line;
        struct {
            SYMBOL *constants;
            char name[255];
            SYMBOL *cs;
        } mesh;
        struct {
            SYMBOL *p;
            double val;
        } set;
        struct {
            double d[4];
            SYMBOL *p;
        } move;
        struct {
            double d[4];
            SYMBOL *p;
        } scale;
        struct {
            double axis;
            double degrees;
            SYMBOL *p;
        } rotate;
        struct {
            SYMBOL *p;
        } basename;
        struct {
            SYMBOL *p;
        } save_knobs;
        struct {
            double start_frame, end_frame;
            SYMBOL *knob_list0;
            SYMBOL *knob_list1;
        } tween;
        struct {
            double num_frames;
        }frames;
        struct {
            SYMBOL *p;
            double start_frame, end_frame, start_val, end_val;
        } vary;
        struct {
            SYMBOL *p;
        } save;
        struct {
            SYMBOL *p;
        } shading;
        struct {
            double value;
        } setknobs;
        struct {
            double value;
        } focal;
    } op;
};

extern struct command op[MAX_COMMANDS];

void print_pcode();
void my_main();

#endif //PARSER_H

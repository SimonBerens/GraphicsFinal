#ifndef SYM_H
#define SYM_H


#include <variant>
#include <memory>
#include "../matrices/m_matri.h"
#include "../scalables/scalable.h"
#include "../scalables/P.h"
#include "../scalables/color.h"


struct Surface {
    Surface(const Scalable<double, 3> &ambient, const Scalable<double, 3> &diffuse,
            const Scalable<double, 3> &specular);

    Scalable<double, 3> ambient;
    Scalable<double, 3> diffuse;
    Scalable<double, 3> specular;
};


//struct SymData {
//    enum Type {
//        MODIFIER,
//        SURFACE,
////        EQUATION, // todo uncomment
//                VALUE
//    };
//
//    SymData(M_Matrix modifier) : modifier(modifier) { // todo reference
//        t = MODIFIER;
//    }
//
//    SymData(Surface surface) : surface(surface) { // todo reference
//        t = SURFACE;
//    }
//
//    Type t;
//    union {
//        M_Matrix modifier;
//        Surface surface; // todo pointerize
////        std::string equation;
//        double value;
//    };
//};

typedef std::variant<Surface, M_Matrix, double> SymData;

#endif //SYM_H

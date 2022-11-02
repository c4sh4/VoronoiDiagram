#ifndef VORONOIDIAGRAM_VEC_H
#define VORONOIDIAGRAM_VEC_H

#pragma once
//STL
#include <iostream>

class Vec {
public:
    double x; // x coordinate
    double y; // y coordinate
    explicit Vec(double x = 0, double y = 0);

    Vec& operator+=(const Vec& vec);
    Vec& operator-=(const Vec& vec);
    bool operator==(const Vec& vec) const;
    double getDet(const Vec& vec) const;
    double getDot(const Vec& vec) const;
    double getNorm() const;
};
Vec operator+(Vec lhs, const Vec& rhs);
Vec operator-(Vec lhs, const Vec& rhs);
std::ostream& operator<<(std::ostream& os,const Vec& vec);

#endif //VORONOIDIAGRAM_VEC_H

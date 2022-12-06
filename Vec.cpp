#include "Vec.h"
//STL
#include <cmath>

//constructor Vec
Vec::Vec(double x, double y): x(x), y(y) {}
//unary operations
Vec& Vec::operator+=(const Vec &vec) {
    x += vec.x;
    y += vec.y;
    return  *this;
}
Vec& Vec::operator-=(const Vec &vec) {
    x -= vec.x;
    y -= vec.y;
}
//binary operations
Vec operator+(Vec& lhs, const Vec& rhs) {
    lhs+=rhs;
    return lhs;
}
Vec operator-(Vec& lhs, const Vec& rhs) {
    lhs-=rhs;
    return lhs;
}
//base operations
double Vec::getDet(const Vec &vec) const {
    return x*vec.y - y*vec.x;
}

//
double Vec::getDot(const Vec &vec) const {
    return x*vec.x + y*vec.y;
}
double Vec::getNorm() const {
    return std::sqrt(x*x + y*y);
}

bool Vec::operator==(const Vec &vec) const {
    if(this->x == vec.x && this->y == vec.y) {
        return true;
    } else return false;
}

//ostream
std::ostream& operator<<(std::ostream& os, const Vec& vec){
    os << "(" << vec.x << ", "  << vec.y << ")" << std::endl;
    return os;
}

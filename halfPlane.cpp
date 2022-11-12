#include "halfPlane.h"
//STL
#include <cmath>
#include <algorithm>
#include <vector>

//methods for class halfPlane
halfPlane::halfPlane(Vec p1, Vec p2, Vec ip) {
    A = p2.y - p1.y;
    B = p1.x - p2.x;
    C = p1.y *  p2.x - p1.x * p2.y;
    site = ip;
    if (pointStatus(ip) < 0){
        A = -A;
        B = -B;
        C = -C;
    }
}
//
halfPlane::halfPlane(): A(),B(),C(),site() {}
//
double halfPlane::pointStatus(Vec &vec) const {
    return A * vec.x + B * vec.y + C;
}
//
std::ostream& operator<<(std::ostream& s, const halfPlane& hp) {
    s << "ABC " << "(" << hp.A << ", " << hp.B << ", " << hp.C << ")" << std::endl;
    return s;
}
//methods for class polytopes
polytopes::polytopes(): mPlanes(),  mVec() {}
//comparator for quicksort
bool cmp(halfPlane& hp1, halfPlane& hp2) {
    Vec v1(hp1.A, hp1.B);
    Vec v2(hp2.A, hp2.B);
    return v1.getDet(v2) > 0;
}
//sort
void polytopes::sortPolytopes() {
    if (mPlanes.size() > 1) {
        std::vector<halfPlane> upper{};
        std::vector<halfPlane> lower{};
        std::vector<halfPlane> check{};
        for (auto &hp: mPlanes) {
            if (hp.B > 0 || (hp.B == 0 && hp.A > 0)) {
                upper.emplace_back(hp);
            } else {
                lower.emplace_back(hp);
            }
        }
        std::sort(upper.begin(), upper.end(), cmp);
        std::sort(lower.begin(), lower.end(), cmp);
        mPlanes.clear();
        mPlanes.shrink_to_fit();
        check.resize(upper.size() + lower.size());
        std::move(upper.begin(), upper.end(), check.begin());
        std::move(lower.begin(), lower.end(), check.begin() + upper.size());
        checkIncl(check);
    }
}
//
void polytopes::checkIncl(std::vector<halfPlane>& ch) {
    for (int i = 0; i < ch.size() - 1; ++i) {
        if (isCollinear(ch[i], ch[i + 1]) && !oppositeSide(ch[i], ch[i + 1])) {
            halfPlane min = ch[i];
            do {
                if (min.C > ch[i+1].C) //min.C > ch[i+1].C // simCoefficient(ch[i+1], min)
                {
                    min = ch[i+1];
                }
                ++i;
            } while (isCollinear(ch[i], ch[i+1]) && !oppositeSide(ch[i], ch[i + 1]));
            mPlanes.emplace_back(min);
            continue;
        }
        else {
            mPlanes.emplace_back(ch[i]);//min
        }
    }
    if (isCollinear(mPlanes.back(), ch[ch.size()-1])) {
        if ((mPlanes.back().C>ch[ch.size()-1].C) && !oppositeSide(mPlanes.back(), ch[ch.size()-1]))  // simCoefficient(ch[i+1], min)
        {
            mPlanes.pop_back();
            mPlanes.emplace_back(ch[ch.size()-1]);
        } else if (oppositeSide(mPlanes.back(), ch[ch.size()-1]))
        {
            mPlanes.emplace_back(ch[ch.size()-1]);
        }
    } else {
        mPlanes.emplace_back(ch[ch.size()-1]);
    }
}
bool oppositeSide(halfPlane& hp1, halfPlane& hp2) {
    Vec v1(hp1.A, hp1.B);
    Vec v2(hp2.A, hp2.B);
    if (v1.getDot(v2) < 0){
        return true;
    } else {
        return false;
    }
}
//
bool afterSort(halfPlane& hp1, halfPlane& hp2, halfPlane& hp3) {
    Vec v1(hp1.A, hp1.B);
    Vec v2(hp2.A, hp2.B);
    Vec v3(hp3.A, hp3.B);
    //std::cout << "v1 dot v2 : " <<v1.getDot(v2) << std::endl;
    //std::cout << "v1 dot v3 : " << v1.getDot(v3) << std::endl;
    if (v1.getDot(v2) > 0){
        if (v1.getDot(v2) < v1.getDot(v3)){
            return true;
        } else return false;
    } else return false;
}
//
//special function to check non-obvious exceptions
bool simCoefficient(halfPlane& hp1, halfPlane& hp2) {
    if (hp1.A != 0) {
        if (hp1.A * hp2.C == hp2.A * hp1.C) {
            return fabs(hp1.C) * fabs(hp2.A) < fabs(hp2.C) * fabs(hp1.A);
        } else {
            return hp1.C < hp2.C;
        }
    } else {
        if (hp1.B * hp2.C == hp2.B * hp1.C) {
            return fabs(hp1.C) * hp2.B < fabs(hp2.C) * hp1.B;
        } else {
            return hp1.C < hp2.C;
        }
    }

}
//
bool isCollinear(halfPlane &hp1, halfPlane &hp2) {
    Vec v1 (hp1.A, hp1.B);
    Vec v2 (hp2.A, hp2.B);
    return (v1.getDet(v2) == 0);
}
//
/*void polytopes::boundaryBox() {
    double inf = 1e9;
    mPlanes = std::vector<halfPlane>();
    Vec center {0,0}; // center of  the box
    Vec l (-inf,-inf);
    Vec u (-inf,inf);
    Vec r (inf,inf);
    Vec d (inf,-inf);
    mPlanes.emplace_back(halfPlane(l , u, center));
    mPlanes.emplace_back(halfPlane(u , r, center));
    mPlanes.emplace_back(halfPlane(r , d, center));
    mPlanes.emplace_back(halfPlane(l , d, center));
}*/
points::points(): mPoints() {}
//
bool cmpPoints(Vec& point1, Vec& point2){
    if (point1.x != point2.x) {
        return point1.x > point2.x;
    } else {
        return point1.y > point2.y;
    }
}
//
void points::sortPoints() {
    std::sort(mPoints.begin(),mPoints.end(), cmpPoints);
}
//

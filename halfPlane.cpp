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
polytopes polytopes::sortPolytopes() {
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
        std::vector<halfPlane> sorted_check{};
        polytopes new_pol;
        sorted_check = reindex(mPlanes);
        new_pol.mPlanes = sorted_check;
        new_pol.mVec = mVec;
        return new_pol;
    }
}

//
std::vector<halfPlane> polytopes::reindex(std::vector<halfPlane>& ch) {
    int k = 0;
    int brek = 0;
    unsigned int size = ch.size();
    for (int i = 0;i < ch.size()-1;++i)
    {
        Vec v1(ch[i].A, ch[i].B);
        Vec v2(ch[i+1].A,ch[i+1].B);
        if (v1.getDet(v2) <= 0) {
            brek = 1;
            break;
        }
        k+=1;
    }
    Vec v3(ch.at(ch.size()-2).A, ch.at(ch.size()-2).B);
    Vec v4(ch.back().A,ch.back().B);
    k+=1;

    if (v3.getDet(v4) <= 0 && brek==0){
        k-=1;
    }

    if (k != ch.size())
    {

        std::vector<halfPlane> ch2;
        int i = 0;
        for (int j =0; j < ch.size();++j)
        {
            unsigned int z;
            z = (i+k)%size;
            ch2.push_back(ch[z]);
            ++i;
        }
        return ch2;
    }
    else
    {
        return ch;
    }
}
//
void polytopes::checkIncl(std::vector<halfPlane>& ch) {
    for (int i = 0; i < ch.size() - 1; ++i) {
        if (isCollinear(ch[i], ch[i + 1]) && !oppositeSide(ch[i], ch[i + 1])) {
            halfPlane min = ch[i];
            //std::cout << mVec << std::endl;
            do {
                if (simCoefficient(min, ch[i+1], mVec)) //min.C > ch[i+1].C // dist1 > dist2
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
        if (simCoefficient(mPlanes.back(), ch[ch.size()-1], mVec) && !oppositeSide(mPlanes.back(), ch[ch.size()-1]))  // simCoefficient(ch[i+1], min)
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
    if (v1.getDet(v2) > 0 && v1.getDet(v2) < v1.getDet(v3))
    {
            return true;
    }
    else if (v1.getDet(v3) > 0 && v2.getDet(v3) < 0 && fabs(v1.getDot(v3)) > fabs(v2.getDot(v3))) {
            //std::cout << "=)" << std::endl;
            return true;
        }
    else return false;
}
//
//special function to check non-obvious exceptions
bool simCoefficient(halfPlane& hp1, halfPlane& hp2, Vec& site) {
    double dist1, dist2;
    dist1 = (fabs(hp1.A * site.x + hp1.B * site.y + hp1.C))/(sqrt(hp1.A*hp1.A+hp1.B * hp1.B));
    dist2 = (fabs(hp2.A * site.x + hp2.B * site.y + hp2.C))/(sqrt(hp2.A*hp2.A+hp2.B * hp2.B));
    return(dist1 > dist2);
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

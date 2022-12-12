#ifndef VORONOIDIAGRAM_HALFPLANE_H
#define VORONOIDIAGRAM_HALFPLANE_H

#pragma once
//STL
#include <iostream>
#include <vector>
//myIncludes
#include "Vec.h"

// Ax+By+C>=0
class halfPlane {
public:
    double A;
    double B;
    double C;
    Vec site; //interior point of halfPlane
    //constructor
    halfPlane();
    halfPlane(Vec p1, Vec p2, Vec ip);  // p1 and p2 - pair of points, ip - interior point
    //check function
    double pointStatus(Vec& vec) const; // status of random point - interior or not
    //
};
std::ostream& operator<<(std::ostream& os, const halfPlane& hp);
bool oppositeSide(halfPlane& hp1, halfPlane& hp2);
bool afterSort(halfPlane& hp1, halfPlane& hp2, halfPlane& hp3);
//collections of halfPlanes

class polytopes{
public:
    std::vector<halfPlane> mPlanes;
    //
    Vec mVec;
    //constructor
    polytopes();
    //sort by normal
    polytopes sortPolytopes();
    //method for tracking of included half-planes
    void checkIncl(std::vector<halfPlane>& ch);
    //boundary box
    //void boundaryBox();
    std::vector<halfPlane> reindex(std::vector<halfPlane> &ch);
};
bool isCollinear(halfPlane& hp1, halfPlane& hp2);
bool simCoefficient(halfPlane& hp1, halfPlane& hp2, Vec& site);

//clas of sites from input
class points{
public:
    //
    std::vector<Vec> mPoints;
    //constructor
    points();
    //sort
    void sortPoints();
    //create all half planes for point
    std::vector<polytopes> getPolytopes();
    //
};

#endif //VORONOIDIAGRAM_HALFPLANE_H
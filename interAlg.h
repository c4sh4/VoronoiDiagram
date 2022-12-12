#ifndef VORONOIDIAGRAM_INTERALG_H
#define VORONOIDIAGRAM_INTERALG_H

#pragma once
//STL
#include <iostream>
#include <vector>
#include <deque>
//myIncludes
#include "halfPlane.h"

//
class interAlg {
public:
    std::deque<halfPlane> deq;
    std::vector<Vec> vertex;
    Vec post;
    //constructor
    interAlg();
    //build intersection of half-planes in a n(log(n))
    void getIntersection(polytopes& pt);
    //vertex calculation
    static Vec vert(halfPlane& hp1, halfPlane& hp2);
    //checking a point for belonging to a half-plane
    static bool checkin(halfPlane& hp1, halfPlane& hp2, halfPlane& plane);
    //
    void getVertexes();
    //
    void Voronoi(std::vector<polytopes> cells);
    //
    bool checkinOld(halfPlane& hp1, halfPlane& hp2, halfPlane& plane);
    //
    Vec FirstVert();

    bool Collinear(halfPlane &hp1, halfPlane &hp2);

    bool checkForLastDot(halfPlane &hp1, halfPlane &hp2);

    bool checkForLastDot(halfPlane &front, halfPlane &back, halfPlane &candidate);
};
//ostream
std::ostream& operator<<(std::ostream& os, const interAlg& alg);
halfPlane getPerpendicular(Vec &sitef, Vec &point);

#endif //VORONOIDIAGRAM_INTERALG_H

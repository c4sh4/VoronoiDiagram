#include "interAlg.h"
//STL
#include <cmath>

//
interAlg::interAlg(): deq(), vertex(), post() {}
//
void interAlg::getIntersection(polytopes& pt)
{
    for (auto hp : pt.mPlanes)
    {
        while (deq.size() > 1 && !checkin(deq.at(deq.size()-2), deq.back(), hp))
        {
            if ((oppositeSide(deq.at(deq.size()-2), deq.back()) && isCollinear(deq.at(deq.size()-2), deq.back())))
            {
                //std::cout<< "HP1 == " << deq.back() << std::endl;
            }
            deq.pop_back();
        }
        while (deq.size() > 1 && !checkin(deq.at(1), deq.front(), hp))
        {
            if (oppositeSide(deq.at(1), deq.front()) && isCollinear(deq.at(1), deq.front()))
            {
                //std::cout <<"HP2 == " << deq.front() << std::endl;
            }
            deq.pop_front();
        }
        deq.emplace_back(hp);
    }
    while (deq.size() > 2 && !checkin(deq.front(), deq.at(1), deq.back()))
    {
        deq.pop_back();
    }
    while (deq.size() > 2 && !checkin(deq.back(), deq.at(deq.size() - 2), deq.front()))
    {
        deq.pop_front();
    }
}
//
Vec interAlg::vert(halfPlane &hp1, halfPlane &hp2) {
    double x, y;
    x = (-hp1.C * hp2.B + hp2.C * hp1.B)/(hp1.A * hp2.B - hp2.A * hp1.B);
    y = (-hp1.A * hp2.C + hp2.A * hp1.C)/(hp1.A * hp2.B - hp2.A * hp1.B);
    Vec apex(x,y);
    return apex;
}
//
bool interAlg::checkin(halfPlane& hp1, halfPlane& hp2, halfPlane& plane) {
    Vec vector = vert(hp1, hp2);

    if (oppositeSide(hp1, hp2) && isCollinear(hp1, hp2)) {
        return true;
    }
    else if ((plane.pointStatus(vector) >= 0 )) {
        return true;
    }
    else  return false;
}
//
bool checkinOld(halfPlane& hp1, halfPlane& hp2, halfPlane& plane) {
    Vec vector = interAlg::vert(hp1, hp2);
    return ((plane.pointStatus(vector) >= 0));
}
//
void interAlg::getVertexes() {
    auto it = deq.begin();
    if (deq.size() > 2) {
        int i =1;
        Vec firstVert = vert(deq.at(0), deq.at(1));
        vertex.emplace_back(vert(deq.at(0), deq.at(1)));
        do {
            if ((firstVert == vert(deq.at(i), deq.at(i+1))))
            {
                deq.erase((it+1));
                --i;
                it--;
            } else {
                vertex.emplace_back(vert(deq.at(i), deq.at(i+1)));
                firstVert = vert(deq.at(i), deq.at(i+1));
            }
            ++i;
            it++;
        } while (i < deq.size()-1 && it < deq.end());
        if (!(firstVert == vert(deq.front(), deq.back())))
        {
            vertex.emplace_back(vert(deq.front(), deq.back()));
        }
        else
        {
            deq.erase((it+1));
        }
    }  else {
        vertex.emplace_back(vert(deq.front(), deq.back()));
    }
}
//
std::ostream& operator<<(std::ostream& os, const interAlg& alg) {
    os << "" << "" << "" << std::endl;
    return os;
}
//
halfPlane getPerpendicular(Vec& sitef, Vec& point){
    halfPlane dist (sitef, point, sitef);
    halfPlane perp;
    double x = (sitef.x + point.x) / 2;
    double y = (sitef.y + point.y) / 2;
    perp.A = (dist.B);
    perp.B = -(dist.A);
    perp.C = (y*dist.A - x*dist.B);
    perp.site = sitef;
    return perp;
}
//
std::vector<polytopes> points::getPolytopes() {
    if(mPoints.size()>1) {
        sortPoints();
        std::vector<polytopes> cells; // cells of diagram for every site
        for(auto & mPoint : mPoints)
        {   polytopes pol;
            halfPlane hp;
            for(auto & j : mPoints)
            {
                if (mPoint == j) { }
                else {
                    hp = getPerpendicular(mPoint, j);
                    //std::cout << "HalfPlane: " << hp << std::endl;
                    pol.mVec = hp.site;
                    pol.mPlanes.emplace_back(hp);
                }
            }
            cells.emplace_back(pol);
        }
        return cells;
    }
    std::cout<< "You have entered one point - the site is the entire plane" << std::endl;
    return {};
}

void interAlg::Voronoi(std::vector<polytopes> cells) {
    std::vector<interAlg> VorAlg;
    for(auto & cell : cells) {
        interAlg alg_for_site;
        cell.sortPolytopes();
        alg_for_site.getIntersection(cell);
        alg_for_site.post = cell.mVec;
        alg_for_site.getVertexes();
        VorAlg.emplace_back(alg_for_site);
    }
    for (auto & j : VorAlg) {
        std::cout << "Site: "<< j.post << std::endl;
        for (auto & i : j.vertex) {
            std::cout << "Vertex: " << i << std::endl;
        }
        for (auto & i : j.deq) {
            std::cout << "Polytopes: " << i << "\n";
        }
    }
}

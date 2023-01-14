#include "interAlg.h"
//STL
#include <cmath>
#include <vector>

//
interAlg::interAlg(): deq(), vertex(), post() {}
//
void interAlg::getIntersection(polytopes& pt)
{
    for (auto hp : pt.mPlanes)
    {
        while (deq.size() > 1 && !checkinOld(deq.at(deq.size()-2), deq.back(), hp))
        {
            deq.pop_back();
        }
        while (deq.size() > 1 && !checkinOld(deq.at(1), deq.front(), hp))
        {
            deq.pop_front();
        }
        deq.emplace_back(hp);
    }
    while (deq.size() > 2 && !checkinOld(deq.back(), deq.at(deq.size() - 2), deq.front()))
    {

        deq.pop_back();
    }

    while (deq.size() > 2 && !checkinOld(deq.front(), deq.at(1), deq.back()))
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
    else if (afterSort(hp1, hp2, plane)){
        return true;
    }
    else  return false;
}
//
bool interAlg::checkinOld(halfPlane& hp1, halfPlane& hp2, halfPlane& plane) {
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
bool interAlg::InfNanVertex(halfPlane& hp1, halfPlane& hp2) {
    if (!(vert(hp1, hp2).x == vert(hp1, hp2).x
       && vert(hp1, hp2).y == vert(hp1, hp2).y))
    {
        return false;
    }
    else if (std::isinf(vert(hp1, hp2).x)
              && std::isinf(vert(hp1, hp2).y))
    {
        return false;
    }
    return true;
}
//
void interAlg::getVertexes() {
    auto it = deq.begin();
    std::vector<halfPlane> truth_res;
    int flag_for_last_vert = 0;
    if (deq.size() > 2) {
        Vec firstVert;
        int i = 0;
        int par = 0;
        int ff_m = int(deq.size());
        int f_m = 0; //flag for check first dot
        while (ff_m >=3)
        {
            if (checkForFirstDot(deq.front(), deq.back(), deq.at(1))
            && deq.size() >= 3)
            {
                deq.pop_front();
                f_m = 1;
                it+=1;
            }
            ff_m-=1;
        }
        int marker_it_for_first_vert = 0;
        for (int m=0, p=1, a=2; m < deq.size(); ++p, ++a)
        {
            if (p == deq.size()){
                p = 0;
            }
            if (a == deq.size()){

                a = 0;
            }
            marker_it_for_first_vert += 1;
            if (!checkForLastDot(deq.at(m), deq.at(p), deq.at(a)))
            {
                firstVert = vert(deq.at(m), deq.at(p));
                truth_res.push_back(deq[m]);
                truth_res.push_back(deq[p]);
                i=p; // !!!!!!!!!
                it+=marker_it_for_first_vert; // !!!!!!!!!!!!!
                par += 1;
                break;
            }
            else
            {
                    deq.erase(it+1);
                    it+=1;
                    --i;
                    --a;
                    --p;
            }
            if (deq.size() <=2){
                if (par < 1){
                    truth_res.push_back(deq[0]);
                    truth_res.push_back(deq[1]);
                    firstVert = vert(deq.at(0), deq.at(1));
                }
                break;
            }
        }

        if ( i == deq.size()){
            i=0;
        }
        vertex.emplace_back(firstVert);
        int it_count = 0-marker_it_for_first_vert;
        if (deq.size() > 2) {
            do {
                if (firstVert == vert(deq.at(i), deq.at(i + 1))) {
                    if (vert(deq.at(i-1), deq.at(i)) == vert(deq.at(i), deq.at(i+1))){
                        truth_res.pop_back();
                        it_count += 1;
                    }
                } else {
                    if (InfNanVertex(deq.at(i), deq.at(i + 1))) {
                        vertex.emplace_back(vert(deq.at(i), deq.at(i + 1)));
                        firstVert = vert(deq.at(i), deq.at(i + 1));
                    }
                }
                ++i;
                truth_res.push_back(deq[i]);
            } while (i < deq.size() - 1);


            if (checkForLastDot(deq.front(), deq.at(deq.size() - 2), deq.back())) {
                truth_res.pop_back();
                deq.pop_back();
            }
            if (firstVert == vert(deq.front(), deq.back())){
                flag_for_last_vert = 1;
            }
            if (!(firstVert == vert(deq.front(), deq.back())) && InfNanVertex(deq.front(), deq.back())) {
                for (int j = 1; j < deq.size() - 1; ++j) {
                    if (checkinOld(deq.front(), deq.back(), deq[j]) &&
                        !(vert(deq.front(), deq.back()) == vertex.back())) {
                        flag_for_last_vert = 1;
                        vertex.emplace_back(vert(deq.front(), deq.back()));
                    }
                }
            }
        }

        std::cout << "Site: "<< deq[0].site << std::endl;
        for (int qq = 0; qq<truth_res.size()-1;++qq){
            get_vertexes_fast(truth_res[qq], truth_res[qq+1]);
        }
        if (flag_for_last_vert == 1)
        {
        get_vertexes_fast(truth_res.back(), truth_res.front());
        }
        for (int qq = 0; qq<truth_res.size();++qq){
            std::cout << "Polytopes: " << truth_res[qq] << "\n";
        }
    }  else {
        truth_res.push_back(deq[0]);
        std::cout << "Site: "<< deq[0].site << std::endl;
        if (deq.size()==2){
            truth_res.push_back(deq[1]);
            get_vertexes_fast(truth_res.front(), truth_res.back());
            std::cout << "Polytopes: " << truth_res[0] << "\n";
            std::cout << "Polytopes: " << truth_res[1] << "\n";
        } else if (deq.size() == 1){
            std::cout << "Polytopes: " << truth_res[0] << "\n";
        }
    }
}
void interAlg::get_vertexes_fast(halfPlane& front, halfPlane& back){
    if (InfNanVertex(front, back)){
        std::cout << "Vertex: " << vert(front, back) << std::endl;
    }
}

bool interAlg::checkForLastDot(halfPlane& front, halfPlane& back, halfPlane& candidate){
    return(vert(front, back) == vert(front, candidate) && vert(front, candidate) == vert(back, candidate));
}
bool interAlg::checkForFirstDot(halfPlane& front, halfPlane& back, halfPlane& candidate){
    if (vert(front, back) == vert(front, candidate) && vert(front, candidate) == vert(back, candidate)) {
        Vec f(front.A, front.B);
        Vec b(back.A, back.B);
        Vec c(candidate.A, candidate.B);
        if (f.getDet(b) <= 0) {
            return true;
        }
        else return false;
    }
    else return false;

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
        polytopes new_cell;
        //cell.sortPolytopes();
        new_cell = cell.sortPolytopes();
        alg_for_site.getIntersection(new_cell);
        alg_for_site.post = cell.mVec;
        alg_for_site.getVertexes();
        VorAlg.emplace_back(alg_for_site);
    }
}

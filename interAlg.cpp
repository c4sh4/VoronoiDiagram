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
            if ((oppositeSide(deq.at(deq.size()-2), deq.back()) && isCollinear(deq.at(deq.size()-2), deq.back())))
            {
                //std::cout<< "HP1 == " << deq.back() << std::endl;
            }
            //std::cout<<"iter: " << hp << std::endl;
            //std::cout<<"pop_back in while1: " << deq.back() << std::endl;
            deq.pop_back();
        }
        while (deq.size() > 1 && !checkinOld(deq.at(1), deq.front(), hp))
        {
            if (oppositeSide(deq.at(1), deq.front()) && isCollinear(deq.at(1), deq.front()))
            {
                //std::cout <<"HP2 == " << deq.front() << std::endl;
            }
            //std::cout<<"pop_front in while: " << deq.front() << std::endl;
            deq.pop_front();
        }
        deq.emplace_back(hp);
    }
    /////////////////////////
    while (deq.size() > 2 && !checkinOld(deq.back(), deq.at(deq.size() - 2), deq.front()))
    {
        //вырезаем здесь!
        //std::cout<<"pop_back in while2: " << deq.front() << std::endl;
        deq.pop_back();
    }

    while (deq.size() > 2 && !checkinOld(deq.front(), deq.at(1), deq.back()))
    {
        //std::cout<<"pop_front in while2: " << deq.back() << std::endl;
        deq.pop_front();
    }

    /////////////////////////
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
    /*else if (afterSort(hp1, hp2, plane)){
        return true;
    }*/
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
Vec interAlg::FirstVert(){
    Vec firstVert;
    int p = 1;
    int a = 2;
    for (int m=0; m < deq.size(); ++m)
    {
        if (checkinOld(deq.at(m), deq.at(p), deq.at(a)))
        {
            firstVert = vert(deq.at(m), deq.at(m+1));
            std::cout << "m: " << m << " firstVert: " << firstVert << std::endl;
            //std::cout << "vert in verushka: " << vert(deq.at(0), deq.at(1)) << std::endl;
            //it++;
            ++p;
            ++a;
            if (p == deq.size()){
                p = 0;
            }
            if (a == deq.size()){
                a = 0;
            }
            break;
        }
    }
    return firstVert;
}
//
void interAlg::getVertexes() {
    auto it = deq.begin();
    if (deq.size() > 2) {
        Vec firstVert;
        int i = 1;
        int par = 0;
        for (int m=0, p=1, a=2; m < deq.size(); ++m, ++p, ++a)
        {
            /*
            std::cout << "---------------- " << std::endl;
            std::cout << "deq is: " << deq.at(m) << std::endl;
            std::cout << "deq size is: " << deq.size() << std::endl;
            std::cout << "m is: " << m << std::endl;
            std::cout << "deq at m is: " << deq.at(m) << std::endl;
            std::cout << "deq at m+1 is: " << deq.at(m+1) << std::endl;
            */
            if (p == deq.size()){
                p = 0;
            }
            if (a == deq.size()){
                a = 0;
            }
            if (checkinOld(deq.at(m), deq.at(p), deq.at(a)))
            {
                if (!isCollinear(deq.at(m), deq.at(m+1))){
                firstVert = vert(deq.at(m), deq.at(m+1));
                //std::cout << "m: " << m << " firstVert: " << firstVert << std::endl;
                //std::cout << "vert in vertushka: " << vert(deq.at(0), deq.at(1)) << std::endl;
                //it++;
                par = m;
                break;}
            }
        }

        //std::cout << "firstVert!!" << firstVert << std::endl;
        vertex.emplace_back(firstVert);
        do {

            if (afterSort(deq.at(i-1), deq.at(i), deq.at(i+1)) && !checkinOld(deq.at(i-1), deq.at(i), deq.at(i+1))){
                //std::cout << " !!! " << vertex.back() << std::endl;
                //vertex.pop_back();
            }
            // Удаление полуплоскости, которая не принадлежить ячейке, но проходит через одну из вершин!
            // (!) не комментить
            else if ((firstVert == vert(deq.at(i), deq.at(i+1))) && par != i)
            {
                //std::cout << "=) sluchilsya erase" << std::endl;
                //std::cout << "=) " <<(it+1)->A<<", " <<(it+1)->B << ", "<< (it+1)->C << std::endl;
                deq.erase((it+1));
                --i;
                it--;
            }
            else {
                if(checkinOld(deq.at(i), deq.at(i+1), deq.at(i-1)) && par != i)
                {
                    vertex.emplace_back(vert(deq.at(i), deq.at(i+1)));
                    //std::cout << "emplace in else : " << vert(deq.at(i), deq.at(i+1)) << std::endl;
                    firstVert = vert(deq.at(i), deq.at(i+1));
                }
            }
            ++i;
            it+=1;
        } while (i < deq.size()-1);


        if (checkForLastDot(deq.front(), deq.at(deq.size()-2), deq.back()))
        {
            //std::cout << "back " << deq.back() << std::endl;
            //std::cout << "back-1 " <<  deq.at(deq.size()-2) << std::endl;
            //std::cout << "SITE " << deq.back().site << std::endl;
            //std::cout << "zabavnoye peresecheniye" << std::endl;
            //std::cout << "=) " << deq.back() << std::endl;
            deq.pop_back();
            --i;
            it--;
        }

        if (!(firstVert == vert(deq.front(), deq.back())))
        {
            for(int j =1; j < deq.size()-1;++j) {
                if (checkinOld(deq.front(), deq.back(), deq[j]) && !(vert(deq.front(), deq.back()) == vertex.back())){
                    //std::cout <<"emplace in if = " << vert(deq.front(), deq.back()) <<std::endl;
                    vertex.emplace_back(vert(deq.front(), deq.back()));}
            }
        }
    }  else {
        vertex.emplace_back(vert(deq.front(), deq.back()));
    }
}
//
bool interAlg::checkForLastDot(halfPlane& front, halfPlane& back, halfPlane& candidate){
    return(vert(front, back) == vert(front, candidate) && vert(front, candidate) == vert(back, candidate));
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
        //new_cell.resize(cell.sortPolytopes().size());
        //std::move(cell.sortPolytopes().begin(), cell.sortPolytopes().end(), new_cell.begin());
        /*
        check.resize(upper.size() + lower.size());
        std::move(upper.begin(), upper.end(), check.begin());
        std::move(lower.begin(), lower.end(), check.begin() + upper.size());*/
        /*
         *
        for(int k = 0; k <new_cell.mPlanes.size(); k++){
            std::cout << "!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << "Cell is:" << cell.mVec << std::endl;
            std::cout << cell.mPlanes[k] << std::endl;
        }*/
        polytopes new_cell;
        //cell.sortPolytopes();
        new_cell = cell.sortPolytopes();
        alg_for_site.getIntersection(new_cell);
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

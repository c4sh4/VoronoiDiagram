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
            //std::cout << "m: " << m << " firstVert: " << firstVert << std::endl;
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
bool interAlg::InfNanVertex(halfPlane& hp1, halfPlane& hp2) {
    if (!(vert(hp1, hp2).x == vert(hp1, hp2).x
       && vert(hp1, hp2).y == vert(hp1, hp2).y))
    {
        //std::cout << "v == v " << vert(hp1, hp2) << std::endl;
        return false;
    }
    else if (std::isinf(vert(hp1, hp2).x)
              && std::isinf(vert(hp1, hp2).y))
    {
        //std::cout << "v isinf " << vert(hp1, hp2) << std::endl;
        return false;
    }
    //std::cout << "v "<< vert(hp1, hp2) << std::endl;
    return true;
}
//
void interAlg::getVertexes() {
    auto it = deq.begin();
    //std::cout<<"vertexes" << std::endl;
    if (deq.size() > 2) {
        Vec firstVert;
        int i = 0;
        int par = 0;
        //checkForFirstDot =)
        if (checkForLastDot(deq.front(), deq.at(1), deq.back()) && deq.size() > 3)
        {
            //std::cout << "SITE First " << deq.front().site << std::endl;
            //std::cout << "back " << deq.back() << std::endl;
            //std::cout << "front " << deq.front() << std::endl;
            //std::cout << "front+1 " <<  deq.at(1) << std::endl;
            //std::cout << "zabavnoye peresecheniye" << std::endl;
            //std::cout << "=) " << deq.back() << std::endl;
            //deq.erase(it+1);
        }
        for (int m=0, p=1, a=2; m < deq.size(); ++p, ++a)
        {
            /*
            std::cout << "---------------- " << std::endl;
            std::cout << "deq is: " << deq.at(m) << std::endl;
            std::cout << "deq size is: " << deq.size() << std::endl;
            std::cout << "m is: " << m << std::endl;
            std::cout << "deq at m is: " << deq.at(m) << std::endl;
            std::cout << "deq at m+1 is: " << deq.at(m+1) << std::endl;


            std::cout<< deq.front().site << std::endl;
            std::cout<<"p " << p <<std::endl;
            std::cout<<"a " << a<<std::endl;
            */

            if (p == deq.size()){
                //std::cout<<"p" << p <<std::endl;
                p = 0;
            }
            if (a == deq.size()){

                a = 0;
            }
            it++;
            ++i;

            /*
            std::cout << it->A << " " << it->B << " " << it->C << std::endl;
            std::cout << deq.at(m);
            std::cout << deq.at(p);
            std::cout << deq.at(a);
            std::cout << "mp vert " <<vert(deq.at(m), deq.at(p)) << std::endl;
            std::cout << "ma vert "<<vert(deq.at(m), deq.at(a)) << std::endl;
            */
            if (!checkForLastDot(deq.at(m), deq.at(p), deq.at(a)))
            {
                firstVert = vert(deq.at(m), deq.at(p));
                //std::cout << "pl: " << deq.at(m).site << std::endl;
                //std::cout << "m: " << m << " firstVert: " << firstVert << std::endl;
                //std::cout << "second hp: " << deq.at(p).A << " " << deq.at(p).B << " " << deq.at(p).C << " firstVert: " << firstVert << std::endl;
                //std::cout << "vert in vertushka: " <<firstVert << std::endl;
                par += 1;
                break;
            }
            else
            {
                //std::cout<< "erase in first Vert: " << it->A << " " << it->B << " " << it->C << std::endl;
                deq.erase(it);
                //it--; //???
                --i;
                --a;
                --p;
                //std::cout << i << std::endl;
            }
            if (deq.size() <=2){
                //std::cout <<"deq.size() <=2" <<std::endl;
                if (par < 1){
                    firstVert = vert(deq.at(0), deq.at(1));
                }
                break;
            }

        }
        if ( i == deq.size()){
            i=0;
        }
        //std::cout << "firstVert!! " << firstVert << std::endl;
        vertex.emplace_back(firstVert);

        //std::cout << "it before while: " << it->A << ", " << it->B << ", " << it->C << std::endl;
        //std::cout << "i before while: " << i << std::endl;

        //std::cout << "proverka x : " << firstVert.x << " "<< vert(deq.at(i), deq.at(i + 1)).x << std::endl;
        //std::cout << "proverka y : " << firstVert.y << " "<< vert(deq.at(i), deq.at(i + 1)).y << std::endl;

        if (deq.size() > 2) {
            //std::cout << "deq.size > 2 :" << deq.size() << std::endl;
            do {
                // Удаление полуплоскости, которая не принадлежить ячейке, но проходит через одну из вершин!
                // (!) не комментить
                //checkForLastDot(deq.at(i), deq.at(i+1), deq.at(i-1))
                //std::cout << firstVert << std::endl;

                if (firstVert == vert(deq.at(i), deq.at(i + 1))) {
                    if (vert(deq.at(i-1), deq.at(i)) == vert(deq.at(i), deq.at(i+1))){
                        deq.erase(it);
                        //std::cout << "i-1 x i == i x i+1" << std::endl;
                        --i;
                        it--; //?
                    } else {
                    //std::cout << "first " << firstVert << std::endl;
                    //std::cout << "site erase " << (it)->site << std::endl;
                    //std::cout << "it in erase " <<(it)->A<<", " <<(it)->B << ", "<< (it)->C << std::endl;
                    //std::cout << "it+1 for erase " << (it + 1)->A << ", " << (it + 1)->B << ", " << (it + 1)->C << std::endl;
                    deq.erase((it + 1));
                    --i;
                    it -= 2;
                    //std::cout << "new it" <<(it+1)->A<<", " <<(it+1)->B << ", "<< (it+1)->C << std::endl;
                    }
                } else {
                    //std::cout << "proverili pokooddinatno: " << std::endl;
                    if (InfNanVertex(deq.at(i), deq.at(i + 1))) {
                        //std::cout << "emplace in else : " << vert(deq.at(i), deq.at(i + 1)) << std::endl;
                        vertex.emplace_back(vert(deq.at(i), deq.at(i + 1)));
                        firstVert = vert(deq.at(i), deq.at(i + 1));
                    }
                }

                ++i;
                it++;
                //std::cout << "it after cycle: " << (it)->A << ", " << (it)->B << ", " << (it)->C << std::endl;

            } while (i < deq.size() - 1 && deq.size() > 2);


            if (checkForLastDot(deq.front(), deq.at(deq.size() - 2), deq.back())) {
                //std::cout << "back " << deq.back() << std::endl;
                //std::cout << "back-1 " <<  deq.at(deq.size()-2) << std::endl;
                //std::cout << "SITE last" << deq.back().site << std::endl;
                //std::cout << "zabavnoye peresecheniye" << std::endl;
                //std::cout << "=) " << deq.back() << std::endl;
                //std::cout << "in Check for lastDot: " << (it)->A << ", " << (it)->B << ", " << (it)->C << std::endl;
                deq.pop_back();
            }
            //check na nan\inf
            if (!(firstVert == vert(deq.front(), deq.back())) && InfNanVertex(deq.front(), deq.back())) {
                for (int j = 1; j < deq.size() - 1; ++j) {
                    if (checkinOld(deq.front(), deq.back(), deq[j]) &&
                        !(vert(deq.front(), deq.back()) == vertex.back())) {

                        //std::cout <<"site in emplace " << it->site <<std::endl;
                        //std::cout <<"emplace in if = " << vert(deq.front(), deq.back()) <<std::endl;
                        vertex.emplace_back(vert(deq.front(), deq.back()));
                    }
                }
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
    /*
    std::cout << "Site: "<< VorAlg[3].post << std::endl;
    for (auto & i : VorAlg[3].vertex) {
        std::cout << "Vertex: " << i << std::endl;
    }
    for (auto & i : VorAlg[3].deq) {
        std::cout << "Polytopes: " << i << "\n";
    }
     */
}

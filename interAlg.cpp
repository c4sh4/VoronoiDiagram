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
    std::vector<halfPlane> truth_res;
    //std::cout << deq.size() << std::endl;
    //std::cout << deq[0].site << std::endl;
    int flag_for_last_vert = 0;
    /*
    for (int i_ = 0; i_ < deq.size(); ++i_){
        std::cout << deq[i_].A << " , " << deq[i_].B << " , " << deq[i_].C << std::endl;
    }*/
    //std::cout << "-----------" << std::endl;

    //work excellent
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
                //std::cout << "deq front for delete: " << deq.front() << std::endl;
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
                //std::cout << "first hp:" << deq[m] << std::endl;
                //std::cout << "second hp:" << deq[p] << std::endl;
                truth_res.push_back(deq[m]);
                truth_res.push_back(deq[p]);
                i=p; // !!!!!!!!!
                it+=marker_it_for_first_vert; // !!!!!!!!!!!!!
                par += 1;
                break;
            }
            else
            {
                    //std::cout<< "erase it+1 =(" << std::endl;
                    deq.erase(it+1);
                    //std::cout << "i is: " << i << std::endl;
                    //std::cout << "marker is: " << marker_it_for_first_vert << std::endl;
                    //std::cout << "hp on mark is:" << deq[marker_it_for_first_vert] << std::endl;
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

        for (int kl = 0; kl < deq.size(); ++kl){
            //std::cout << "before while: " << (it+kl) ->A << ", " << (it+kl)->B << ", " << (it+kl)->C << std::endl;
            //std::cout << kl<< std::endl;
        }

        if (deq.size() > 2) {
            do {
                // Удаление полуплоскости, которая не принадлежить ячейке, но проходит через одну из вершин!
                // (!) не комментить
                if (firstVert == vert(deq.at(i), deq.at(i + 1))) {
                    if (vert(deq.at(i-1), deq.at(i)) == vert(deq.at(i), deq.at(i+1))){
                        //std::cout<< "i erased: " << deq.at(i) << std::endl;
                        truth_res.pop_back();
                        //deq.erase(it);
                        it_count += 1;
                        //i--;
                    }
                } else {
                    //std::cout << "proverili pokooddinatno: " << std::endl;
                    if (InfNanVertex(deq.at(i), deq.at(i + 1))) {
                        //std::cout << deq.at(i) << std::endl;
                        //std::cout << "emplace in else : " << vert(deq.at(i), deq.at(i + 1)) << std::endl;
                        vertex.emplace_back(vert(deq.at(i), deq.at(i + 1)));
                        firstVert = vert(deq.at(i), deq.at(i + 1));
                    }
                }
                ++i;
                truth_res.push_back(deq[i]);
                //std::cout << "hp in while: " << deq[i] << std::endl;
                //std::cout  << "last of deq: " << deq.back() << std::endl;
            } while (i < deq.size() - 1);


            if (checkForLastDot(deq.front(), deq.at(deq.size() - 2), deq.back())) {
                truth_res.pop_back();
                deq.pop_back();
            }

            //check na nan\inf
            /*
            std::cout << "SITE IS: " << deq.back().site << std::endl;
            std::cout << deq.front() <<std::endl;
            std::cout << deq.back() <<std::endl;
            std::cout << InfNanVertex(deq.front(), deq.back()) <<std::endl;
            std::cout << !(firstVert == vert(deq.front(), deq.back())) <<std::endl;
            */
            if (firstVert == vert(deq.front(), deq.back())){
                flag_for_last_vert = 1;
            }
            if (!(firstVert == vert(deq.front(), deq.back())) && InfNanVertex(deq.front(), deq.back())) {
                //std::cout << "_______________" << std::endl;
                //std::cout << "welcome1: " << deq.back().site << std::endl;
                for (int j = 1; j < deq.size() - 1; ++j) {
                    if (checkinOld(deq.front(), deq.back(), deq[j]) &&
                        !(vert(deq.front(), deq.back()) == vertex.back())) {
                        flag_for_last_vert = 1;
                        //std::cout << "_______________" << std::endl;
                        //std::cout << "welcome2: " << deq.back().site << std::endl;
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
bool interAlg::get_vertexes_fast(halfPlane& front, halfPlane& back){
    if (InfNanVertex(front, back)){
        std::cout << "Vertex: " << vert(front, back) << std::endl;
    }
}


//
bool interAlg::checkForLastDot(halfPlane& front, halfPlane& back, halfPlane& candidate){
    return(vert(front, back) == vert(front, candidate) && vert(front, candidate) == vert(back, candidate));
}
bool interAlg::checkForFirstDot(halfPlane& front, halfPlane& back, halfPlane& candidate){
    if (vert(front, back) == vert(front, candidate) && vert(front, candidate) == vert(back, candidate)) {
        Vec f(front.A, front.B);
        Vec b(back.A, back.B);
        Vec c(candidate.A, candidate.B);
        if (f.getDet(b) <= 0) {
            //std::cout << "f.Det(b)<=0" << std::endl;
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
    /*
    for (auto & j : VorAlg) {
        std::cout << "Site: "<< j.post << std::endl;
        for (auto & i : j.vertex) {
            std::cout << "Vertex: " << i << std::endl;
        }
        for (auto & i : j.deq) {
            std::cout << "Polytopes: " << i << "\n";
        }
    }*/
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

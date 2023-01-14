//STL
#include <iostream>
#include <fstream>
//myIncludes
#include "interAlg.h"


int main() {
    int size = 0;
    points sites;
    std::cin >> size;
    for(int i = 0; i < size; ++i){
        double x1, y1;
        std::cin >> x1 >> y1;
        Vec vec{x1,y1};
        sites.mPoints.emplace_back(vec);
        std::cin.clear();
    }
    std::vector<polytopes> cells;
    cells = sites.getPolytopes();
    interAlg test;
    test.Voronoi(cells);
}

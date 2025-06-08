#include "utils.hpp"
#include <vector>
#include <string>
#include <fstream>

namespace utils{
    bool contains(const std::vector<unsigned int>& vec, unsigned int element){
        return std::find(vec.begin(), vec.end(), element) != vec.end();
    }

    unsigned int factorial(unsigned int n){
        if(n == 1)
            return 1;
        return n*factorial(n-1);
    }

    void exportToTxt(const PolyhedronCollection& p_coll, const std::string& path){
        // esporto tutti i file
        vertex::exportTxt(p_coll, path);
        edge::exportTxt(p_coll, path);
        face::exportTxt(p_coll, path);
        polyhedron::exportTxt(p_coll, path);
    }
}
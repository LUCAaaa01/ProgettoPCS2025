#pragma once

#include <iostream>
#include "PolyhedronCollection.hpp"

namespace polyhedron{
    std::vector<Vertex> import_Polyhedron(PolyhedronCollection& p_coll,const std::string& prefix,unsigned int numVertices,unsigned int numEdges,unsigned int numFaces);
}

#include "polyhedron.hpp"
#include "PolyhedronCollection.hpp"
#include "UCDUtilities.hpp"

int main(){

    unsigned int p = 3; 
    unsigned int q = 3;
    unsigned int b = 8;
    PolyhedronCollection p_coll;
    polyhedron::initialize(p_coll, 2);
    unsigned int poly_id = polyhedron::build_platonic_solid(p_coll, p, q);
    poly_id = polyhedron::buildGeodesicClassI(p_coll, poly_id, q, b);

    for(size_t i = 0; i < p_coll.Cell3DsFaces[0].size(); i++)
        std::cout << "Id della faccia: " << p_coll.Cell3DsFaces[0][i] << std::endl;

    for(size_t i = 0; i < p_coll.Cell3DsVertices[0].size(); i++)
        std::cout << "Id dei vertici: " << p_coll.Cell3DsVertices[0][i] << std::endl;

    for(size_t i = 0; i < p_coll.Cell3DsFaces[poly_id].size(); i++)
        std::cout << "****** Id della faccia: " << p_coll.Cell3DsFaces[poly_id][i] << std::endl;

    for(size_t i = 0; i < p_coll.Cell3DsVertices[poly_id].size(); i++)
        std::cout << "***** Id dei vertici: " << p_coll.Cell3DsVertices[poly_id][i] << std::endl;

    std::cout << "Fino a qui tutto okay?" << std::endl;

    poly_id = polyhedron::createDual(p_coll, poly_id, q);

    Eigen::MatrixXd vertices(3, p_coll.Cell3DsVertices[poly_id].size());

    std::cout << "numero di vetici: " << p_coll.Cell3DsVertices[poly_id].size() << std::endl;
    for (size_t i = 0; i < p_coll.Cell3DsVertices[poly_id].size(); ++i) {
        vertices.col(i) = p_coll.Cell0DsCoordinates.col(p_coll.Cell3DsVertices[poly_id][i]);
    }

    Eigen::MatrixXi edges(2, p_coll.Cell3DsEdges[poly_id].size());

    std::cout << "numero di lati: " << p_coll.Cell3DsEdges[poly_id].size() << std::endl;
    for (size_t i = 0; i < p_coll.Cell3DsEdges[poly_id].size(); ++i) {
        edges.col(i) = p_coll.Cell1DsEndpoints.col(p_coll.Cell3DsEdges[poly_id][i]);
    }


    Gedim::UCDUtilities utilities;
    {
        utilities.ExportPoints("./Cell0Ds.inp", p_coll.Cell0DsCoordinates);
        utilities.ExportSegments("./Cell1Ds.inp",
            p_coll.Cell0DsCoordinates,
                                 edges);
    }


    return 0;
}
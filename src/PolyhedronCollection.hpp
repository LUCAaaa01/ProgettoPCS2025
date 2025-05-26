#pragma once

#include <iostream>
#include "Eigen/Eigen"


//namespace PolygonalLibrary 

struct PolyhedronCollection
{
    // ---------- ATTRIBUTI VERTICI --------------- //
    unsigned int NumCell0Ds = 0; 
    std::vector<unsigned int> Cell0DsId = {}; 
    Eigen::MatrixXd Cell0DsCoordinates = {};

    // ---------- ATTRIBUTI SEGMENTI --------------- //
    unsigned int NumCell1Ds = 0; 
    std::vector<unsigned int> Cell1DsId = {}; 
    Eigen::MatrixXi Cell1DsEndpoints = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)

    // ---------- ATTRIBUTI FACCE --------------- //
    unsigned int NumCell2Ds = 0;
    std::vector<unsigned int> Cell2DsId = {};
    std::vector<unsigned int> NumCell2DsVertices;
    std::vector<unsigned int> NumCell2DsEdges;
    std::vector<std::vector<unsigned int>> Cell2DsVertices = {};
    std::vector<std::vector<unsigned int>> Cell2DsEdges = {}; 

    // ---------- ATTRIBUTI POLIEDRI --------------- //
    unsigned int NumCell3Ds = 0; ///< number of Cell2D
    std::vector<unsigned int> Cell3DsId = {}; 
    std::vector<unsigned int> NumCell3DsVertices;
    std::vector<unsigned int> NumCell3DsEdges;
    std::vector<unsigned int> NumCell3DsFaces;
    std::vector<std::vector<unsigned int>> Cell3DsVertices = {}; 
    std::vector<std::vector<unsigned int>> Cell3DsEdges = {}; 
    std::vector<std::vector<unsigned int>> Cell3DsFaces = {};


};

// Verifica se un elemento appartiene ad un vettore
inline bool contains(const std::vector<unsigned int>& vec, unsigned int element){
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

inline unsigned int factorial(unsigned int n){
    if(n == 1)
        return 1;
    return n*factorial(n-1);
}
#pragma once

#include <iostream>
#include "Eigen/Eigen"


//namespace PolygonalLibrary {

struct PolyhedronCollection
{
    // ---------- ATTRIBUTI VERTICI --------------- //
    unsigned int NumCell0Ds = 0; 
    std::vector<unsigned int> Cell0DsId = {}; 
    Eigen::MatrixXd Cell0DsCoordinates = {}; // 3xNumeroCell0Ds

    // ---------- ATTRIBUTI SEGMENTI --------------- //
    unsigned int NumCell1Ds = 0; ///< number of Cell1D
    std::vector<unsigned int> Cell1DsId = {}; ///< Cell1D id, size 1 x NumberCell1D
    Eigen::MatrixXi Cell1DsExtrema = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)

    // ---------- ATTRIBUTI FACCE --------------- //
    unsigned int NumCell2Ds = 0; ///< number of Cell2D
    std::vector<unsigned int> Cell2DsId = {}; ///< Cell2D id, size 1 x NumberCell2D
    std::vector<unsigned int> NumCell2DsVertices;
    std::vector<unsigned int> NumCell2DsEdges;
    std::vector<std::vector<unsigned int>> Cell2DsVertices = {}; ///< Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    std::vector<std::vector<unsigned int>> Cell2DsEdges = {}; ///< Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]

    // ---------- ATTRIBUTI POLIEDRI --------------- //
    unsigned int NumCell3Ds = 0; ///< number of Cell2D
    std::vector<unsigned int> Cell3DsId = {}; ///< Cell2D id, size 1 x NumberCell2D
    std::vector<unsigned int> NumCell3DsVertices;
    std::vector<unsigned int> NumCell3DsEdges;
    std::vector<unsigned int> NumCell3DsFaces;
    std::vector<std::vector<unsigned int>> Cell3DsVertices = {}; ///< Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    std::vector<std::vector<unsigned int>> Cell3DsEdges = {}; ///< Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]
    std::vector<std::vector<unsigned int>> Cell3DsFaces = {};


}
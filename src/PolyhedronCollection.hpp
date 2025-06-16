#pragma once

#include <iostream>
#include "Eigen/Eigen"


struct PolyhedronCollection
{
    // ---------- ATTRIBUTI VERTICI --------------- //
    unsigned int NumCell0Ds = 0; //numero di vertici
    std::vector<unsigned int> Cell0DsId = {}; //id dei vertici
    Eigen::MatrixXd Cell0DsCoordinates = {}; //coordinate x,y,z dei vertici

    // ---------- ATTRIBUTI SEGMENTI --------------- //
    unsigned int NumCell1Ds = 0; //numero di lati
    std::vector<unsigned int> Cell1DsId = {}; //id dei lati
    Eigen::MatrixXi Cell1DsEndpoints = {}; //id del vertice iniziale e del vertice finale del lato

    // ---------- ATTRIBUTI FACCE --------------- //
    unsigned int NumCell2Ds = 0; //numero di facce
    std::vector<unsigned int> Cell2DsId = {}; //id delle facce
    std::vector<unsigned int> NumCell2DsVertices; //numero di vertici nelle diverse facce
    std::vector<unsigned int> NumCell2DsEdges; //numero di lati nelle diverse facce
    std::vector<std::vector<unsigned int>> Cell2DsVertices = {}; //id dei vertici nelle diverse facce
    std::vector<std::vector<unsigned int>> Cell2DsEdges = {}; //id dei lati nelle diverse facce

    // ---------- ATTRIBUTI POLIEDRI --------------- //
    unsigned int NumCell3Ds = 0; //numero di poliedri
    std::vector<unsigned int> Cell3DsId = {}; //id dei poliedri
    std::vector<unsigned int> NumCell3DsVertices; //numero di vertici nei diversi poliedri
    std::vector<unsigned int> NumCell3DsEdges; //numero di lati nei diversi poliedri
    std::vector<unsigned int> NumCell3DsFaces; //numero di facce nei diversi poliedri
    std::vector<std::vector<unsigned int>> Cell3DsVertices = {}; //id dei vertici nei diversi poliedri
    std::vector<std::vector<unsigned int>> Cell3DsEdges = {}; //id dei lati nei diversi poliedri
    std::vector<std::vector<unsigned int>> Cell3DsFaces = {}; //id delle facce nei diversi poliedri


};
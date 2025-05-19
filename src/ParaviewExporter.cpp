#pragma once

#include <vector>
#include <fstream>
#include "PolyhedronCollection.hpp"

void writeVTK(const PolyhedronCollection& p, const std::string& filename) {
    std::ofstream out(filename);
    out << "# vtk DataFile Version 3.0\n";
    out << "Polyhedron vertices and edges\n";
    out << "ASCII\nDATASET POLYDATA\n";
    
    // Stampa dei punti
    int N = p.numVertices();
    out << "Punti " << N << " float\n";
    for (int i = 0; i < N; ++i) {
        auto [x,y,z] = p.vertex(i);
        out << x << " " << y << " " << z << "\n";
    }
    
    // Stampa dei lati
    auto edges = p.allEdges(); //
    int M = edges.size();
    // Formato: <num_points_per_cell> <pt0> <pt1>
    out << "Lati " << M << " " << (M * 3) << "\n";
    for (auto [u,v] : edges)
        out << 2 << " " << u << " " << v << "\n";
    
    out.close();
}


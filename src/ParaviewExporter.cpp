#include <vector>
#include <fstream>
#include "PolyhedronCollection.hpp"

void writeVTK(const Polyhedron& P, const std::string& filename) {
    std::ofstream out(filename);
    out << "# vtk DataFile Version 3.0\n";
    out << "Polyhedron vertices and edges\n";
    out << "ASCII\nDATASET POLYDATA\n";
    
    // 1) punti
    int N = P.numVertices();
    out << "POINTS " << N << " float\n";
    for (int i = 0; i < N; ++i) {
        auto [x,y,z] = P.vertex(i);
        out << x << " " << y << " " << z << "\n";
    }
    
    // 2) linee
    auto edges = P.allEdges(); // vector<pair<int,int>>
    int M = edges.size();
    // formato: <num_points_per_cell> <pt0> <pt1>
    out << "LINES " << M << " " << (M * 3) << "\n";
    for (auto [u,v] : edges)
        out << 2 << " " << u << " " << v << "\n";
    
    out.close();
}


#pragma once

#include <gtest/gtest.h>
#include "PolyhedronCollection.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "face.hpp"
#include "polyhedron.hpp"
#include "Eigen/Eigen"

class PolyhedronTest : public ::testing::Test { 
protected:
  PolyhedronCollection p;
  unsigned int tetId;

  void SetUp() override {
    // Costruisco tetraedro platonico base: {p=3, q=3}
    tetId = polyhedron::build_platonic_solid(p, 3, 3);
    // Verifico che sia stato creato un solo poliedro 
    ASSERT_EQ(p.NumCell3Ds, 1u);
    // Conservo gli ID delle facce per i test dopo
    f0 = p.Cell3DsFaces[tetId][0];
    f1 = p.Cell3DsFaces[tetId][1];
    f2 = p.Cell3DsFaces[tetId][2];
    f3 = p.Cell3DsFaces[tetId][3];
  }

  unsigned int f0, f1, f2, f3;
};

// Verifico che la reshape conservativa mantenga i dati già esistenti
TEST_F(PolyhedronTest, ReshapePreservesDataWhenRequested) {
  polyhedron::add(p, {f0, f1, f2, f3});
  auto old_size = p.NumCell3Ds;
  polyhedron::reshape(p, 20, true);
  EXPECT_EQ(p.NumCell3Ds, old_size);
  EXPECT_GE(p.Cell3DsId.capacity(), 20u);
}

// Verifica che la reshape non conservativa resetti il tutto
TEST_F(PolyhedronTest, ReshapeDropsDataWhenNotPreserved) {
  polyhedron::add(p, {f0, f1, f2, f3});
  polyhedron::reshape(p, 5, false);
  EXPECT_EQ(p.NumCell3Ds, 0u);
  EXPECT_TRUE(p.Cell3DsFaces.empty());
}

// Verifico che isEqual rilevi due poliedri con stesse facce (l'ordine è irrilevante)  
TEST_F(PolyhedronTest, IsEqualRecognizesSameFaces) {
  std::vector<unsigned int> p1 = {f0, f1, f2, f3};
  std::vector<unsigned int> p2 = {f3, f1, f0, f2};
  EXPECT_TRUE(polyhedron::isEqual(p1, p2));
}

// Verifico che getId trovi il poliedro che c'è già
TEST_F(PolyhedronTest, GetIdFindsExistingPolyhedron) {
  polyhedron::add(p, {f0, f1, f2, f3});
  unsigned int id = polyhedron::getId(p, {f0, f1, f2, f3});
  EXPECT_EQ(id, 0u);
}

// Verifico che add eviti duplicati se richiesto
TEST_F(PolyhedronTest, AddAvoidsDuplicatesWithCheck) {
  unsigned int id1 = polyhedron::add(p, {f0, f1, f2, f3});
  unsigned int id2 = polyhedron::add(p, {f0, f1, f2, f3}, true);
  EXPECT_EQ(id1, id2);
  EXPECT_EQ(p.NumCell3Ds, 1u);
}

// Verifico che add crei nuovo poliedro se check=false
TEST_F(PolyhedronTest, AddCreatesNewIfCheckDisabled) {
  unsigned int id1 = polyhedron::add(p, {f0, f1, f2, f3}, false);
  unsigned int id2 = polyhedron::add(p, {f0, f1, f2, f3}, false);
  EXPECT_NE(id1, id2);
  EXPECT_EQ(p.NumCell3Ds, 3u);
}

TEST_F(PolyhedronTest, CreateDualOfCubeYieldsOctahedron) {
  // Importo un cubo (p=4,q=3)
  unsigned int cubeId = polyhedron::build_platonic_solid(p, 4, 3);
  ASSERT_EQ(p.Cell3DsId.size(), 2u);
  EXPECT_EQ(p.NumCell3DsVertices[cubeId], 8u);
  EXPECT_EQ(p.NumCell3DsEdges[cubeId], 12u);
  EXPECT_EQ(p.NumCell3DsFaces[cubeId], 6u);

  // Creo il duale
  unsigned int dualId = polyhedron::createDual(p, cubeId, 3);
  // ora NumCell3Ds dovrebbe essere 3
  EXPECT_EQ(p.NumCell3Ds, 3u);

  // Verifico i conteggi del duale
  EXPECT_EQ(p.NumCell3DsVertices[dualId], 6u);
  EXPECT_EQ(p.NumCell3DsEdges[dualId], 12u);
  EXPECT_EQ(p.NumCell3DsFaces[dualId], 8u);
}

TEST_F(PolyhedronTest, BuildClassI_IncreasesVertices) {
  // Inizializzo il poliedro platonico: ad esempio p=3 (tetraedro), q=3
  unsigned int base_poly_id = polyhedron::build_platonic_solid(p, 3, 3);

  // Numero di vertici prima dell'espansione
  size_t initial_vertex_count = p.Cell3DsVertices[base_poly_id].size();

  // Costruzione geodetica di classe I con b = 2
  unsigned int new_poly_id = polyhedron::buildGeodesicClassI(p, base_poly_id, 3, 2);

  // Verifico che il numero di vertici sia aumentato
  size_t new_vertex_count = p.Cell3DsVertices[new_poly_id].size();
  EXPECT_GT(new_vertex_count, initial_vertex_count);
}

//buildGeodesicClassII su un solido platonico di base
TEST_F(PolyhedronTest, BuildClassII_IncreasesVertices) {
    unsigned int base_poly_id = polyhedron::build_platonic_solid(p, 3, 4); // ottaedro

    size_t initial_vertex_count = p.Cell3DsVertices[base_poly_id].size();

    unsigned int new_poly_id = polyhedron::buildGeodesicClassII(p, base_poly_id, 4, 2);

    size_t new_vertex_count = p.Cell3DsVertices[new_poly_id].size();
    EXPECT_GT(new_vertex_count, initial_vertex_count);
}

// createGeodesicPolyhedron restituisce un poliedro valido
TEST_F(PolyhedronTest, CreateGeodesicPolyhedron_ReturnsValidID) {
    // Costruisco un poliedro geodetico di classe I con p=3, q=3, b=1, c=0
    unsigned int poly_id = polyhedron::createGeodesicPolyhedron(p, 3, 3, 1, 0);

    // Verifichiamo che l'ID restituito sia presente nella collezione
    EXPECT_LT(poly_id, p.Cell3DsFaces.size());
    EXPECT_FALSE(p.Cell3DsVertices[poly_id].empty());
}

// createGoldbergPolyhedron restituisce un poliedro valido
TEST_F(PolyhedronTest, CreateGoldbergPolyhedron_ReturnsValidID) {
    // Costruisco un poliedro di Goldberg con p=3, q=3, b=1, c=0
    unsigned int poly_id = polyhedron::createGoldbergPolyhedron(p, 3, 3, 1, 0);

    EXPECT_LT(poly_id, p.Cell3DsFaces.size());
    EXPECT_FALSE(p.Cell3DsVertices[poly_id].empty());
}

// findShortestPath con stesso vertice (distanza zero)
TEST_F(PolyhedronTest, FindShortestPath_SameVertex) {
    // Creiamo un cubo base (p=4, q=3)
    unsigned int poly_id = polyhedron::build_platonic_solid(p, 4, 3);

    // Prendo il primo vertice
    int v0 = p.Cell3DsVertices[poly_id][0];
    std::vector<int> vertices_path;
    std::vector<int> edges_path;
    double dist = polyhedron::findShortestPath(p, poly_id, 0, 0, vertices_path, edges_path, false);

    EXPECT_DOUBLE_EQ(dist, 0.0);
    // Il path deve contenere esattamente un vertice (se stesso)
    ASSERT_EQ(vertices_path.size(), 1u);
    EXPECT_EQ(vertices_path[0], v0);
    EXPECT_TRUE(edges_path.empty());
}


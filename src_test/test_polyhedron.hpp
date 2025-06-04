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

// Verifico che initialize resetti correttamente lo stato
TEST_F(PolyhedronTest, InitializeEmptiesAllVectors) {
  polyhedron::initialize(p, 10);
  EXPECT_EQ(p.NumCell3Ds, 0u);
  EXPECT_TRUE(p.Cell3DsId.empty());
  EXPECT_TRUE(p.Cell3DsFaces.empty());
  EXPECT_TRUE(p.Cell3DsVertices.empty());
}

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

// Verifica che il conteggio dei vertex, faces e edges cresca correttamente.
TEST_F(PolyhedronTest, BuildGeodesicClassIOnTetrahedron) {
  // crea un tetraedro (p=3,q=3)
  unsigned int tetId = polyhedron::build_platonic_solid(p, 3, 3);
  ASSERT_EQ(p.NumCell3Ds, 1u);

  // Applico geodesic classe I con b=1
  unsigned int geo1 = polyhedron::buildGeodesicClassI(p, tetId, 3, 1);
  EXPECT_EQ(p.NumCell3DsVertices[geo1], p.NumCell3DsVertices[tetId]);
  EXPECT_EQ(p.NumCell3DsEdges[geo1],    p.NumCell3DsEdges[tetId]);
  EXPECT_EQ(p.NumCell3DsFaces[geo1],    p.NumCell3DsFaces[tetId]);

  // Lo applico con b=2: ogni spigolo viene suddiviso in 2 parti
  unsigned int geo2 = polyhedron::buildGeodesicClassI(p, tetId, 3, 2);
  // Per un tetraedro originale (4v,6e,4f): vertici=6, lato=12 e facce=16
  EXPECT_EQ(p.NumCell3DsVertices[geo2], 10u);
  EXPECT_EQ(p.NumCell3DsEdges[geo2],    12u);
  EXPECT_EQ(p.NumCell3DsFaces[geo2],    16u);
}


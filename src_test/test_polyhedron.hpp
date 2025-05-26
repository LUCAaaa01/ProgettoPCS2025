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

  void SetUp() override {
    vertex::initialize(p, 4);
    edge::initialize(p, 6);
    face::initialize(p, 4);

    // Creo un tetraedro
    vertex::add(p, Eigen::Vector3d(1, 1, 1));
    vertex::add(p, Eigen::Vector3d(-1, -1, 1));
    vertex::add(p, Eigen::Vector3d(-1, 1, -1));
    vertex::add(p, Eigen::Vector3d(1, -1, -1));

    // Creo le 4 facce del tetraedro come triangoli
    f0 = face::add(p, {0, 1, 2});
    f1 = face::add(p, {0, 1, 3});
    f2 = face::add(p, {0, 2, 3});
    f3 = face::add(p, {1, 2, 3});
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
  EXPECT_EQ(p.NumCell3Ds, 2u);
}

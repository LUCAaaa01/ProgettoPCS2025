#pragma once

#include <gtest/gtest.h>
#include "PolyhedronCollection.hpp"
#include "face.hpp"
#include "Eigen/Eigen"

class FaceTest : public ::testing::Test {
protected:
  PolyhedronCollection p;

  void SetUp() override {
    // Inizializza con capacità per 5 facce
    face::initialize(p, 5);
    // Prepara 4 punti per le facce
    p.Cell0DsCoordinates.resize(3, 5);
    p.Cell0DsCoordinates.col(0) << 0, 0, 0;
    p.Cell0DsCoordinates.col(1) << 1, 0, 0;
    p.Cell0DsCoordinates.col(2) << 1, 1, 0;
    p.Cell0DsCoordinates.col(3) << 0, 1, 0;

    p.NumCell0Ds = 4;
    p.Cell0DsId = {0, 1, 2, 3};

    edge::initialize(p, 12);
  }
};

// Controlla l’aggiunta di facce e che duplicati (ordinamento diverso) non vengano replicati
TEST_F(FaceTest, AddFacesAndPreventDuplicates) {
  unsigned int f0 = face::add(p, {0,1,2});
  unsigned int f1 = face::add(p, {2,1,3});
  EXPECT_EQ(f0, 0u);
  EXPECT_EQ(f1, 1u);
  EXPECT_EQ(p.NumCell2Ds, 2u);

  // un ordine diverso dello stesso triangolo dovrebbe ritornare f0
  unsigned int dup = face::add(p, {1,2,0});
  EXPECT_EQ(dup, f0);
  EXPECT_EQ(p.NumCell2Ds, 2u);
}

// Verifica che getCentroid crei un nuovo punto e ne calcoli correttamente le coordinate
TEST_F(FaceTest, GetCentroidCreatesNewPoint) {
  unsigned int f = face::add(p, {0,1,2});
  unsigned int cid = face::getCentroid(p, f, false);
  EXPECT_EQ(cid, p.NumCell0Ds - 1);
  Eigen::Vector3d c = p.Cell0DsCoordinates.col(cid);
  EXPECT_NEAR(c(0), 2.0/3.0, 1e-6);
  EXPECT_NEAR(c(1), 1.0/3.0, 1e-6);
}

// Controlla la rilevazione di adiacenza tra due facce che condividono un lato
TEST_F(FaceTest, DetectsAdjacency) {
  unsigned int f0 = face::add(p, {0,1,2});
  unsigned int f1 = face::add(p, {2,1,3});
  EXPECT_EQ(face::areAdjacent(p, f0, f1), true);
}

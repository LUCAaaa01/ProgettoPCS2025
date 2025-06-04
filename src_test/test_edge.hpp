#pragma once

#include <gtest/gtest.h>
#include "PolyhedronCollection.hpp"
#include "edge.hpp"
#include "Eigen/Eigen"

class EdgeTest : public ::testing::Test {
protected:
  PolyhedronCollection p;

  void SetUp() override {
    // Inizializza con capacità per 5 segmenti
    edge::initialize(p, 5);
    // Prepara 4 punti per la lunghezza dei segmenti
    p.Cell0DsCoordinates.resize(3,4);
    p.Cell0DsCoordinates.col(0) << 0, 0, 0;
    p.Cell0DsCoordinates.col(1) << 1, 0, 0;
    p.Cell0DsCoordinates.col(2) << 1, 1, 0;
    p.Cell0DsCoordinates.col(3) << 0, 1, 0;

    p.NumCell0Ds = 4;
    p.Cell0DsId = {0, 1, 2, 3};
  }
};

// Verifica che l’inizializzazione riservi la capacità corretta e NumCell1Ds=0
TEST_F(EdgeTest, InitializeSetsCapacity) {
  EXPECT_GE(p.Cell1DsId.capacity(), 5u);
  EXPECT_EQ(p.NumCell1Ds, 0u);
}

// Controlla l’aggiunta di lati e che i duplicati (ordine invertito) non vengano duplicati
TEST_F(EdgeTest, AddEdgesAndPreventDuplicates) {
  unsigned int e0 = edge::add(p, Eigen::Vector2i(0,1));
  unsigned int e1 = edge::add(p, Eigen::Vector2i(1,2));
  EXPECT_EQ(e0, 0u);
  EXPECT_EQ(e1, 1u);
  EXPECT_EQ(p.NumCell1Ds, 2u);

  // aggiunge 1–0 (dup di 0–1), deve ritornare e0 senza incrementare count
  unsigned int dup = edge::add(p, Eigen::Vector2i(1,0));
  EXPECT_EQ(dup, e0);
  EXPECT_EQ(p.NumCell1Ds, 2u);
}

// Verifica correttezza del calcolo della lunghezza del segmento (√2)
TEST_F(EdgeTest, LengthCalculation) {
  unsigned int e = edge::add(p, Eigen::Vector2i(0,2));
  double len = edge::length(p, e);
  EXPECT_NEAR(len, std::sqrt(2.0), 1e-8);
}

// Test reshape non conservativa: NumCell1Ds deve azzerarsi e la matrice adattarsi
TEST_F(EdgeTest, ReshapeNonConservativeClearsOrAdjusts) {
  edge::add(p, Eigen::Vector2i(0,1));
  edge::reshape(p, 2, false);
  EXPECT_EQ(p.NumCell1Ds, 0u);
  EXPECT_EQ(p.Cell1DsEndpoints.cols(), 2);
}

#pragma once

#include <gtest/gtest.h>
#include "PolyhedronCollection.hpp"
#include "vertex.hpp"  
#include "Eigen/Eigen"

class VertexTest : public ::testing::Test {
protected:
  PolyhedronCollection p;

  // la funzione override inizializza tutte le risorse che servono prima di ogni test
  void SetUp() override {
    // preparo p con capacità iniziale
    vertex::initialize(p, 5);
  }
};

// ------ SPIEGAZIONE TEST -------
// EXPECT_GE: "Greater or Equal"; controlla che il primo parametro sia maggiore o uguale rispetto al secondo ; se non lo è fallisce e compare un messaggio nel log
// EXPECT_EQ : "Equality"; controlla che il primo parametro sia uguale al secondo; se i due non sono uguali, Google Test stampa entrambi

// Verifica che l’inizializzazione riservi almeno la capacità richiesta e imposti count=0
TEST_F(VertexTest, InitializeSetsCapacity) {
  EXPECT_GE(p.Cell0DsId.capacity(), 5u);
  EXPECT_EQ(p.NumCell0Ds, 0u);
}

// Controlla che l’aggiunta di nuovi punti incrementi correttamente NumCell0Ds e restituisca gli ID attesi
TEST_F(VertexTest, AddNewPointsIncreasesCount) {
  unsigned int id0 = vertex::add(p, Eigen::Vector3d(0,0,0));
  unsigned int id1 = vertex::add(p, Eigen::Vector3d(1,0,0));
  EXPECT_EQ(id0, 0u);
  EXPECT_EQ(id1, 1u);
  EXPECT_EQ(p.NumCell0Ds, 2u);
}

// Assicura che aggiungere un punto con stesse coordinate non ne crei uno nuovo, ma ritorni l’ID esistente
TEST_F(VertexTest, AddDuplicatePointReturnsExistingId) {
  unsigned int id0 = vertex::add(p, Eigen::Vector3d(1,1,0));
  unsigned int idDup = vertex::add(p, Eigen::Vector3d(1,1,0));  // controllo = true di default
  EXPECT_EQ(idDup, id0);
  EXPECT_EQ(p.NumCell0Ds, 1u);
}
// Verifica che la proiezione sulla sfera normalizzi il raggio a 1
TEST_F(VertexTest, ProjectionLeadsToUnitRadius) {
  unsigned int id = vertex::add(p, Eigen::Vector3d(2,0,0));
  unsigned int pid = vertex::projectOnSphere(p, id);
  Eigen::Vector3d coords = p.Cell0DsCoordinates.col(pid);
  double r = coords.norm();
  EXPECT_NEAR(r, 1.0, 1e-6);
}

// Controlla che la distanza tra due punti sia calcolata correttamente
TEST_F(VertexTest, DistanceBetweenPoints) {
  unsigned int a = vertex::add(p, Eigen::Vector3d(0,0,0));
  unsigned int b = vertex::add(p, Eigen::Vector3d(0,3,4));
  double d = vertex::distance(p, a, b);
  EXPECT_NEAR(d, 5.0, 1e-8);
}

// Verifica averagePoints e interpolatePoints 
TEST_F(VertexTest, AverageAndInterpolate) {
  // average of (0,0,0) and (2,0,0) is (1,0,0)
  unsigned int a = vertex::add(p, Eigen::Vector3d(0,0,0));
  unsigned int b = vertex::add(p, Eigen::Vector3d(2,0,0));
  Eigen::VectorXd avg = vertex::averagePoints(p, {a,b});
  EXPECT_NEAR(avg(0), 1.0, 1e-8);
  EXPECT_NEAR(avg(1), 0.0, 1e-8);
  EXPECT_NEAR(avg(2), 0.0, 1e-8);

  Eigen::VectorXd ip = vertex::interpolatePoints(p, a, b, 0.25);
  EXPECT_NEAR(ip(0), 0.5, 1e-8);
}

// Controlla che reshape conservativa mantenga i dati esistenti e aumenti la capacità
TEST_F(VertexTest, ReshapeConservativePreservesData) {
  vertex::add(p, Eigen::Vector3d(1,2,3));
  vertex::reshape(p, 10, true);
  EXPECT_GE(p.Cell0DsId.capacity(), 10u);
  EXPECT_EQ(p.NumCell0Ds, 1u);
  // punto 0 rimane invariato
  EXPECT_EQ(p.Cell0DsCoordinates(0,0), 1.0);
  EXPECT_EQ(p.Cell0DsCoordinates(1,0), 2.0);
  EXPECT_EQ(p.Cell0DsCoordinates(2,0), 3.0);
}

TEST_F(VertexTest, SimpleReflection) {
    p.Cell0DsId = {0u, 1u, 2u};
    p.Cell0DsCoordinates.resize(3, 4);
    p.Cell0DsCoordinates.col(0) = Eigen::Vector3d(0.0, 0.0, 0.0);
    p.Cell0DsCoordinates.col(1) = Eigen::Vector3d(2.0, 0.0, 0.0);
    p.Cell0DsCoordinates.col(2) = Eigen::Vector3d(3.0, 0.0, 0.0);
    p.NumCell0Ds = 3;
    // Mi aspetto che il punto riflesso sia (-1,0,0)
    unsigned int new_id = vertex::reflect(p,0u,1u,2u);
    
    // Il nuovo ID dovrebbe essere 3 (successivo a 0,1,2)
    EXPECT_EQ(new_id, 3u);

    // Verifico che l'id 3 sia stato aggiunto a Cell0DsId
    ASSERT_TRUE(contains(p.Cell0DsId, new_id));

    // Verifico la coordinata calcolata
    Eigen::Vector3d expected(-1.0, 0.0, 0.0);
    Eigen::Vector3d actual = p.Cell0DsCoordinates.col(new_id);
    EXPECT_DOUBLE_EQ(actual.x(), expected.x());
    EXPECT_DOUBLE_EQ(actual.y(), expected.y());
    EXPECT_DOUBLE_EQ(actual.z(), expected.z());
}

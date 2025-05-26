#pragma once

#include <gtest/gtest.h>
#include "PolyhedronCollection.hpp"
#include "face.hpp"
#include "Eigen/Eigen"

class PolyhedronTest : public ::testing::Test {
protected:
  PolyhedronCollection p;

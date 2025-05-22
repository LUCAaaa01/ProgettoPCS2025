#include <gtest/gtest.h>

#include "test_vertex.hpp"
#include "test_edge.hpp"
#include "test_face.hpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

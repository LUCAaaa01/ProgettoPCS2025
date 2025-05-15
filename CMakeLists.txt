cmake_minimum_required(VERSION 3.0.0) 

project(Facsimile LANGUAGES C CXX VERSION 1.0.0) 

set(Pippo Bob)  
message(${Pippo}) 

set(Pippo Bob Alice) 
set(Pipppo "Bob" "Alice") 

list(APPEND Pippo "Fabio") 
message(${Pippo})

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################
## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND ${PROJECT_NAME}_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

## SUBDIRECTORY
add_subdirectory(src) 
add_subdirectory(Export Paraview)
add_subdirectory(src_test)

list(APPEND ${PROJECT_NAME}_TEST_sources ${src_sources} ${src_paraview_sources} ${src_test_sources})
list(APPEND ${PROJECT_NAME}_TEST_headers ${src_sources} ${src_paraview_headers} ${src_test_headers})
list(APPEND ${PROJECT_NAME}_TEST_includes ${src_sources} ${src_paraview_includes} ${src_test_includes})


list(APPEND ${PROJECT_NAME}_sources ${src_sources} ${src_paraview_sources})
list(APPEND ${PROJECT_NAME}_headers ${src_sources} ${src_paraview_headers})
list(APPEND ${PROJECT_NAME}_includes ${src_sources} ${src_paraview_includes})



add_executable(${CMAKE_PROJECT_NAME} main.cpp ${${PROJECT_NAME}_sources} ${${PROJECT_NAME}_headers)

target_link_libraries(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_LINKED_LIBRARIES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_includes})

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -fPIC)

add_executable(${CMAKE_PROJECT_NAME}_TEST main_test.cpp ${${PROJECT_NAME}_TEST_sources} ${${PROJECT_NAME}_TEST_headers)

target_link_libraries(${PROJECT_NAME}_TEST PUBLIC ${${PROJECT_NAME}_LINKED_LIBRARIES})
target_include_directories(${CMAKE_PROJECT_NAME}_TEST PUBLIC ${${PROJECT_NAME}_TEST_includes})



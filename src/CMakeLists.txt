message("SUBFOLDER")
message(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_BINARY_DIR})
message(${CMAKE_SOURCE_DIR}) 
message(${CMAKE_CURRENT_SOURCE_DIR}) 
message(${PROJECT_NAME})
message(${CMAKE_PROJECT_NAME})


list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/vertex.hpp)
list(APPEND src_sources ${CMAKE_CURRENT_SOURCE_DIR}/vertex.cpp)

list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/edge.hpp)
list(APPEND src_sources ${CMAKE_CURRENT_SOURCE_DIR}/edge.cpp)

list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/face.hpp)
list(APPEND src_sources ${CMAKE_CURRENT_SOURCE_DIR}/face.cpp)

list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/polyhedron.hpp)
list(APPEND src_sources ${CMAKE_CURRENT_SOURCE_DIR}/polyhedron.cpp)

list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp)
list(APPEND src_sources ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp)

list(APPEND src_headers ${CMAKE_CURRENT_SOURCE_DIR}/PolyhedronCollection.hpp)


list(APPEND src_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(src_sources ${src_sources} PARENT_SCOPE)
set(src_headers ${src_headers} PARENT_SCOPE)
set(src_includes ${src_includes} PARENT_SCOPE)

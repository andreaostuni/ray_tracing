cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

project(ray_tracing)


add_executable(ray_tracing src/main.cpp)
include_directories(include)


target_link_libraries(ray_tracing)

install(DIRECTORY include/ DESTINATION ${CMAKE_SOURCE_DIR}/install/include)

install(TARGETS ray_tracing 
  RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install/bin
  LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/install/lib
  ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/install/lib
)
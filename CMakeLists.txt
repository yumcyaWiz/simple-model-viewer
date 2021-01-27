cmake_minimum_required(VERSION 3.12)
project(simple-model-viewer LANGUAGES CXX)

add_executable(viewer src/viewer.cpp)
target_compile_features(viewer PRIVATE cxx_std_17)
set_target_properties(viewer PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(viewer PRIVATE src)
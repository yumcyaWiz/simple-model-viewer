cmake_minimum_required(VERSION 3.12)
project(simple-model-viewer LANGUAGES C CXX)

# viewer
add_executable(viewer src/viewer.cpp)
target_compile_features(viewer PRIVATE cxx_std_17)
set_target_properties(viewer PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(viewer PRIVATE src)

# compile options
target_compile_options(viewer PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

# externals
add_subdirectory(externals)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(viewer PRIVATE OpenGL::GL)

# glad
target_link_libraries(viewer PRIVATE glad)

# GLFW
target_link_libraries(viewer PRIVATE glfw)

# glm
target_link_libraries(viewer PRIVATE glm)

# assimp
find_package(ASSIMP REQUIRED)
target_link_libraries(viewer PRIVATE assimp)

# imgui
target_link_libraries(viewer PRIVATE imgui)

# imgui_glfw_opengl3
target_link_libraries(viewer PRIVATE imgui_glfw_opengl3)

# stb
target_link_libraries(viewer PRIVATE stb)
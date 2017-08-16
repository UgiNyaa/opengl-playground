cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall")

project(glpgrnd)

file(GLOB_RECURSE glpgrnd_SOURCES "src/*.cpp")
file(GLOB_RECURSE glpgrnd_HEADERS "src/*.hpp")

file(COPY resources DESTINATION .)

add_executable(glpgrnd ${glpgrnd_SOURCES} ${glpgrnd_HEADERS})
target_link_libraries(glpgrnd GLEW glfw GL assimp png)

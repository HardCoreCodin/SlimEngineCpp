cmake_minimum_required(VERSION 3.8)

project(0_task)
add_executable(0_task WIN32 src/examples/AL_task.cpp)

project(1_viewport)
add_executable(1_viewport WIN32 src/examples/1_viewport.cpp)

project(2_navigation)
add_executable(2_navigation WIN32 src/examples/2_navigation.cpp)

project(3_cameras)
add_executable(3_cameras WIN32 src/examples/3_cameras.cpp)

project(4_shapes)
add_executable(4_shapes WIN32 src/examples/4_shapes.cpp)

project(5_manipulation)
add_executable(5_manipulation WIN32 src/examples/5_manipulation.cpp)

project(6_mesh)
add_executable(6_mesh WIN32 src/examples/6_mesh.cpp)

project(7_scene)
add_executable(7_scene WIN32 src/examples/7_scene.cpp)

project(obj2mesh)
add_executable(obj2mesh src/obj2mesh.cpp)

project(bmp2texture)
add_executable(bmp2texture src/bmp2texture.cpp)
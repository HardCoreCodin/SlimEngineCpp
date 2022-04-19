cmake_minimum_required(VERSION 3.8)

project(SlimEngineCpp_0_barebone)
add_executable(SlimEngineCpp_0_barebone WIN32 src/examples/0_barebone.cpp)

project(SlimEngineCpp_1_viewport)
add_executable(SlimEngineCpp_1_viewport WIN32 src/examples/1_viewport.cpp)

project(SlimEngineCpp_2_navigation)
add_executable(SlimEngineCpp_2_navigation WIN32 src/examples/2_navigation.cpp)

project(SlimEngineCpp_3_cameras)
add_executable(SlimEngineCpp_3_cameras WIN32 src/examples/3_cameras.cpp)

project(SlimEngineCpp_4_shapes)
add_executable(SlimEngineCpp_4_shapes WIN32 src/examples/4_shapes.cpp)

project(SlimEngineCpp_5_manipulation)
add_executable(SlimEngineCpp_5_manipulation WIN32 src/examples/5_manipulation.cpp)

project(SlimEngineCpp_6_mesh)
add_executable(SlimEngineCpp_6_mesh WIN32 src/examples/6_mesh.cpp)

project(SlimEngineCpp_7_scene)
add_executable(SlimEngineCpp_7_scene WIN32 src/examples/7_scene.cpp)

project(obj2mesh)
add_executable(obj2mesh src/obj2mesh.cpp)
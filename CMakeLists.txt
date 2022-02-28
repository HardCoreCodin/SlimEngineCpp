cmake_minimum_required(VERSION 3.8)

#project(PerspectiveProjection)
#add_executable(PerspectiveProjection WIN32 src/examples/visualizations/perspective_projection.cpp)

#project(obj2mesh)
#add_executable(obj2mesh src/obj2mesh.c)
#
#project(bmp2texture)
#add_executable(bmp2texture src/bmp2texture.c)

project(Slim3D_1_viewport)
add_executable(Slim3D_1_viewport WIN32 src/examples/1_viewport.cpp)

project(Slim3D_2_navigation)
add_executable(Slim3D_2_navigation WIN32 src/examples/2_navigation.cpp)

project(Slim3D_3_cameras)
add_executable(Slim3D_3_cameras WIN32 src/examples/3_cameras.cpp)

project(Slim3D_4_shapes)
add_executable(Slim3D_4_shapes WIN32 src/examples/4_shapes.cpp)

project(Slim3D_5_manipulation)
add_executable(Slim3D_5_manipulation WIN32 src/examples/5_manipulation.cpp)

project(Slim3D_6_mesh)
add_executable(Slim3D_6_mesh WIN32 src/examples/6_mesh.cpp)

project(Slim3D_7_scene)
add_executable(Slim3D_7_scene WIN32 src/examples/7_scene.cpp)

#project(Slim3D_8_rasterization)
#add_executable(Slim3D_8_rasterization WIN32 src/examples/8_rasterization.cpp)
<img src="SlimEngineCpp_logo.png" alt="SlimEngine++_logo"><br>

Minimalist base-project for interactive graphical applications (2D/3D)<br>
Written in plain C++ (For the original C variant, see [SlimEngine](https://github.com/HardCoreCodin/SlimEngine) <br>
Pure-software (no hardware acceleration of any kind)<br>
Focuses on simplicity, ease of use and brain-dead setup (single header file, no dependencies)<br>
<br>
Built on: [SlimApp](https://github.com/HardCoreCodin/SlimApp) <br>
Used in: [SlimTracin](https://github.com/HardCoreCodin/SlimTracin) <br>

Architecture:
-
SlimEngine++ is platform-agnostic by design, though currently only supports Windows.<br>
The platform layer only uses operating-system headers - no standard library used.<br>
The application layer itself has no 3rd-party dependencies - only uses standard math headers.<br>
It is just a library that the platform layer uses - it has no knowledge of the platform.<br>

More details on this architecture [here](https://youtu.be/Ev_TeQmus68).

Usage:
-
The single header file variant includes everything.<br>
Otherwise, specific headers can be included from the directory of headers.<br>
The main entry point for the app needs to be defined explicitly (see [SlimApp](https://github.com/HardCoreCodin/SlimApp)). <br>

SlimEngine++ comes with pre-configured CMake targets for all examples.<br>
For manual builds on Windows, the typical system libraries need to be linked<br>
(winmm.lib, gdi32.lib, shell32.lib, user32.lib) and the SUBSYSTEM needs to be set to WINDOWS<br>

All examples were tested in all combinations of:<br>
Compiler: MSVC, MinGW, CLang<br>
Language: C++<br>
CPU Arch: x86, x64<br>

Features:
-
SlimEngine++ extends SlimApp and so inherits all it's features<br>
Additional features include facilities for interactive 3D applications:<br>
* A scene with cameras, lights, materials, textures, meshes and parametric curves<br>
* Scene selection and interactive transformations (moving, rotating and scaling)<br>
* 3D Viewport with a HUD and rich mouse/keyboard navigation<br>
* 3D Line drawing for wireframe rendering (optionally multi-sampled for very clean lines)<br>
* 3D Triangle rasterization:
  - Mesh Shaders
  - Pixel Shaders
  - Vertex attribute interpolation
  - Perspective corrected barycentric coordinates
  - Texture Mip-Mapping (auto-selected mip-level based on UV derivatives)
  - Bi-Linear filtering
  - Triangle clipping
  - Frustum culling (per-vertex/full-triangles)
  - Early back-face culling (optional)
<br>
<img src="src/examples/MSAA.gif" alt="MSAA" height="360"><br>

Well documented example applications cover the features:<br>
* <b><u>3D Viewports:</b></u><br>
  <img src="src/examples/1_viewport_HUD.gif" alt="1_viewport_HUD" height="360"><br>
  Viewports come with a HUD that can be toggled on/off<br>
  SlimEngine++ comes with an empty scene, apart from a default camera bound to a default viewport.<br>
  The window content is bound to the default viewport, so they resize together.<br>
  Customizing the scene/viewport can be done in callbacks that get invoked when they're ready.<br>
  The HUD is fully customizable, and can hold any number of lines set at viewport-initialization time.<br>
  <p float="left">
    <img src="src/examples/1_viewport_updateAndRender_c.png" alt="1_viewport_updateAndRender_code" width="360">
    <img src="src/examples/1_viewport_initialization_c.png" alt="1_viewport_initialization_code" height="300">
    <img src="src/examples/1_viewport_HUD_update_c.png" alt="1_viewport_HUD_update_code" width="360">
    <img src="src/examples/1_viewport_setup_c.png" alt="1_viewport_setup_code" height="300">  
  </p>
  
* <b><u>Viewport</b>:</u> Navigation<br>
  <img src="src/examples/2_navigation.gif" alt="1_scene" height="360"><br>
  Viewport navigation actions can be composed to set-up WASD(FPS)/DCC(orb) navigation style(s).<br>
  Keyboard navigation is agnostic to key-binding and is trivial to map keys to.<br>
  It supports moving forward, backwards, left, right, up and down and turning left or right.<br>
  mouse navigation controls the viewport's camera and is more granular.<br>
  It can pan, zoom, dolly, orient or orbit the camera around a target.<br>
  <p float="left">
    <img src="src/examples/2_navigation_setup_c.png" alt="2_navigation_setup_code" width="350">
    <img src="src/examples/2_navigation_mouse_c.png" alt="2_navigation_mouse_code" height="300">
  </p>

* <b><u>Cameras</b>:</u> Viewport binding<br>
  <img src="src/examples/3_cameras.gif" alt="1_scene" height="360"><br>
  The scene can have any number of cameras which can be bound to any viewport dynamically.<br>
  The camera can be drawn as part of the drawing of the scene<br>
  <p float="left">
    <img src="src/examples/3_cameras_update_c.png" alt="3_cameras_update_code" height="260">
    <img src="src/examples/3_cameras_setup_c.png" alt="3_cameras_setup_code" height="260">
  </p>

* <b><u>Shapes</b>:</u> Setup and drawing parametric geometries (Grid, Box, Curves, etc.)<br>
  <img src="src/examples/4_shapes_rotating.gif" alt="1_scene" height="360"><br>
  SlimEngine++ allocates and initializes the scene based on quantities set at app-initialization time.<br>
  Wireframe drawing of the scene to the viewport can be set-up to occur on window-redraw.<br>  
  <p float="left">
    <img src="src/examples/4_shapes_initialization_c.png" alt="4_scene_initialization_and_viewport_drawing_code" width="350">
    <img src="src/examples/4_shapes_viewport_drawing_c.png" alt="4_scene_viewport_drawing_code" height="300">
  </p>

* <b><u>Scene</b>:</u> Object selection and manipulation (Translation, Rotation and Scale)<br>
  <img src="src/examples/5_manipulation.gif" alt="5_manipulation" height="360"><br>
  Scene objects can be dragged around the screen, moving them parallel to the viewing plane.<br>
  Selecting an object displays it's bouding box, enabling for per-axis translation, rotation and scaling.<br><br>
  <p float="left">
    <img src="src/examples/5_manipulation_update_c.png" alt="4_manipulation_update_code" width="350">
    <img src="src/examples/5_manipulation_scene_setup_c.png" alt="4_manipulation_scene_setup_code" width="350">
  </p>
  
* <b><u>Scene</b>:</u> Mesh loading from file, instancing and wire-frame drawing<br>
  <img src="src/examples/6_mesh_with_normals.gif" alt="6_mesh_with_normals" height="360"><br>
  Meshes can be loaded from binary files, with vertex positions, normals and texture coordinates.<br>
  They can be moves and transformed just like any other kind of geometry.<br>
  <img src="src/examples/6_mesh_instancing.gif" alt="6_mesh_instancing" height="360"><br>
  A mesh can have multiple instances, each with their own transformation and colors.<br>
  Simply have multiple mesh geometries all set with same mesh id.<br>
  Memory is allocated automatically for the meshes by reading their headers before loading.<br>
  <p float="left">
    <img src="src/examples/6_mesh_drawing_c.png" alt="6_mesh_drawing_code" width="350">
    <img src="src/examples/6_mesh_setup_c.png" alt="6_mesh_setup_code" width="350">
  </p>

* <b><u>Scene</b>:</u> Saving to and loading from `.scene` files<br>
  <img src="src/examples/7_scene.gif" alt="7_scene" height="360"><br>
  Scenes can be saved to a file and later loaded back in-place.
  <p float="left">
    <img src="src/examples/7_scene_setup_c.png" alt="7_scene_io_code" width="350">
    <img src="src/examples/7_scene_message_c.png" alt="7_scene_message_code" width="350">
  </p>

* <b><u>obj2mesh</b>:</u> Also privided is a separate CLI tool for converting `.obj` files to `.mesh` files.<br>
  Usage: `./obj2mesh src.obj trg.mesh`<br>
  - invert_winding_order : Reverses the vertex ordering (for objs exported with clockwise order)<br>

* <b><u>bmp2texture</b>:</u> Also provided is a separate CLI tool for converting `.bmp` files to `.texture` files.<br>
  Usage: `./bmp2texture src.bmp trg.texture`<br>
  - m : Generate mip-maps<br>
  - w : Wrap-around<br>
  - f : Filter<br>

<b>SlimEngine++</b> does not come with any GUI functionality at this point.<br>
Some example apps have an optional HUD (heads up display) that shows additional information.<br>
It can be toggled on or off using the`tab` key.<br>

All examples are interactive using <b>SlimEngine++</b>'s facilities having 2 interaction modes:
1. FPS navigation (WASD + mouse look + zooming)<br>
2. DCC application (default)<br>

Double clicking the `left mouse button` anywhere within the window toggles between these 2 modes.<btr>

Entering FPS mode captures the mouse movement for the window and hides the cursor.<br>
Navigation is then as in a typical first-person game (plus lateral movement and zooming):<br>

Move the `mouse` to freely look around (even if the cursor would leave the window border)<br>
Scroll the `mouse wheel` to zoom in and out (changes the field of view of the perspective)<br>
Hold `W` to move forward<br>
Hold `S` to move backward<br>
Hold `A` to move left<br>
Hold `D` to move right<br>
Hold `R` to move up<br>
Hold `F` to move down<br>

Exit this mode by double clicking the `left mouse button`.

The default interaction mode is similar to a typical DCC application (i.e: Maya):<br>
The mouse is not captured to the window and the cursor is visible.<br>
Holding the `right mouse button` and dragging the mouse orbits the camera around a target.<br>
Holding the `middle mouse button` and dragging the mouse pans the camera (left, right, up and down).<br>
Scrolling the `mouse wheel` dollys the camera forward and backward.<br>

Clicking the `left mouse button` selects an object in the scene that is under the cursor.<br>
Holding the `left mouse button` while hovering an object and then dragging the mouse,<br>
moves the object parallel to the screen.<br>

Holding `alt` highlights the currently selecte object by drawing a bounding box around it.<br>
While `alt` is still held, if the cursor hovers the selected object's bounding box,<br>
mouse interaction transforms the object along the plane of the bounding box that the cursor hovers on:<br>
Holding the `left mouse button` and dragging the mouse moves the object.<br>
Holding the `right mouse button` and dragging the mouse rotates the object.<br>
Holding the `middle mouse button` and dragging the mouse scales the object.<br>
<i>(`mouse wheel` interaction is disabled while `alt` is held)</i><br>
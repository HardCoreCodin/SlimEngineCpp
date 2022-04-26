<img src="SlimEngineCpp_logo.png" alt="SlimEngine++_logo"><br>

Minimalist base-project for interactive graphical applications (2D/3D)<br>
Written in plain C++. For the original C variant, see [SlimEngine](https://github.com/HardCoreCodin/SlimEngine)<br>
Pure-software (no hardware acceleration)<br>
Focuses on simplicity, ease of use and setup with no dependencies and an optional single header file variant<br>
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
Alternatively, specific headers can be included from the directory of headers.<br>
<br>
All features are opt-in, only the entry point needs to be defined.<br>
It just needs to return a pointer to an instance of a SlimEngine derived class:<br><br>
<img src="src/examples/0_barebone.png"><br><br>
On its own that yield a valid application with a well-behaved blank window.<br>
<br>
SlimEngine++ comes with pre-configured CMake targets for all examples.<br>
For manual builds on Windows, the typical system libraries need to be linked<br>
(winmm.lib, gdi32.lib, shell32.lib, user32.lib) and the SUBSYSTEM needs to be set to WINDOWS<br>

All examples were tested in all combinations of:<br>
Compiler: MSVC, MinGW, CLang<br>
Language: C++<br>
CPU Arch: x86, x64<br>

Features:
-
SlimEngine++ extends SlimApp++ and so inherits all it's features<br>
Additional features include facilities for interactive 3D applications:<br><br>
* Scene with cameras, geometries, meshes and parametric curves<br>
* Scene selection with interactive transformations (moving, rotating and scaling)<br>
* Customizable Heads Up Display (HUD)<br>
* 3D Viewport with rich mouse/keyboard navigation<br>
* 3D Line drawing for wireframe rendering (optionally multi-sampled for very clean lines)<br>
<br>
<img src="src/examples/MSAA.gif" alt="MSAA"><br><br>

Well documented example applications cover the features:<br><br>

* <b><u>Window:</b></u><br><br>
  Renders a Canvas into its content:<br><br> 
  <img src="src/examples/0_window.png"><br><br>
  <br>

* <b><u>HUD:</b></u><br><br>
  A table of lines with Title and Value columns.<br><br> 
  <img src="src/examples/1_HUDLine.png"><br><br>
  Values can be numeric or strings and can be bound to a boolean for toggling<br><br>
  <img src="src/examples/1_HUD.png"><br><br>
  Line height and text color can be controlled globally or per line<br><br>
  <img src="src/examples/1_HUDSettings.png"><br><br>
  A typical setup might look like this:<br><br>
  <img src="src/examples/1_HUD_setup.png"><br><br>
  Updating the values may look like this:<br><br>
  <img src="src/examples/1_HUD_update.png"><br><br>
  Toggled rendering might look like this:<br><br>
  <img src="src/examples/1_HUD_render.png"><br><br>
  <br>

* <b><u>Viewport:</b></u><br><br>
  <img src="src/examples/1_viewport_HUD.gif"><br><br>
  Binds to a Canvas and a Camera:<br><br> 
  <img src="src/examples/1_viewport_setup.png"><br><br>
  Used for all 3D geometry drawing:<br><br>
  <img src="src/examples/1_viewport_drawing.png"><br><br>
  Can be made to resize with the window:<br><br>
  <img src="src/examples/1_viewport_resize.png"><br><br>
  <br>

* <b><u>Navigation</b>:</u><br><br>
  <img src="src/examples/2_navigation.gif"><br><br>
  Actions available for panning, zooming, dollying, orienting and orbiting the camera:<br><br>
  <img src="src/examples/2_navigation.png"><br><br>
  They compose to provide 1st person shooter or DCC orbiting style navigations:<br><br>
  <img src="src/examples/2_navigation_composed.png"><br><br>
  Navigation modes can be toggled through mouse capture tracking:<br><br>
  <img src="src/examples/2_navigation_mouse.png"><br><br>
  Keyboard navigation supports moving in any direction:<br><br>
  <img src="src/examples/2_navigation_velocity.png"><br><br>
  Movement has acceleration and deceleration:<br><br>
  <img src="src/examples/2_navigation_acceleration.png"><br><br>
  Navigation is agnostic to key-binding and is trivial to map keys to:<br><br>
  <img src="src/examples/2_navigation_keyboard.png"><br><br>
  Navigation is enabled through the Viewport:<br><br>
  <img src="src/examples/2_navigation_update.png"><br><br>
  <br>

* <b><u>Cameras</b>:</u><br><br>
  <img src="src/examples/3_cameras.gif"><br><br>
  There can be any number of cameras in a scene:<br><br>
  <img src="src/examples/3_cameras_setup.png"><br><br> 
  Different cameras can be (re)bound to any viewport at runtime:<br><br>
  <img src="src/examples/3_cameras_binding.png"><br><br>
  Cameras can also be drawn to a viewport:<br><br>
  <img src="src/examples/3_cameras_drawing.png"><br><br>
  <br>

* <b><u>Shapes</b>:</u><br><br>
  <img src="src/examples/4_shapes_rotating.gif"><br>
  Grids, Boxes and Curves contain their parameters, transforms are kept separately:<br><br>
  <img src="src/examples/4_shapes_setup.png"><br><br>
  Transforms can be modified with ease:<br><br>
  <img src="src/examples/4_shapes_update.png"><br><br>
  Wireframe viewport drawing is provided with transforms explicitly:<br><br>
  <img src="src/examples/4_shapes_drawing.png"><br><br>
  <br>
  
* <b><u>Scene Selection and Manipulation</b>:</u><br><br>
  <img src="src/examples/5_manipulation.gif"><br><br>
  Shapes and Transforms can compose into a Scene with Geometry objects:<br><br>
  <img src="src/examples/5_manipulation_setup.png"><br><br>
  A scene Selection model can then be managed alongside a Scene.<br>
  Selected geometries can be dragged along the screen or transformed locally:<br><br>
  <img src="src/examples/5_manipulation_update.png"><br><br>
  Selection highlights can be drawn to the viewport:<br><br>
  <img src="src/examples/5_manipulation_render.png"><br><br>
  <br>

* <b><u>Mesh</b>:</u><br><br>
  <img src="src/examples/6_mesh_with_normals.gif"><br><br>
  Triangular meshes can be loaded from `.mesh` files containing vertex positions, normals and uvs.<br><br>
  <img src="src/examples/6_mesh_setup.png"><br><br>
  Meshes bind to Geometry objects with Transforms and are part of a Scene like any other geometry type.<br>
  Geometry objects can be rebound to different meshes at runtime:<br><br>
  <img src="src/examples/6_mesh_binding.png"><br><br>
  Meshes are instanced by having different Geometry objects bound to the same Mesh.<br><br>
  <img src="src/examples/6_mesh_instancing.gif"><br><br>
  They can then be drawn in different colors and with different transforms:<br><br>
  <img src="src/examples/6_mesh_drawing.png"><br><br>
  <br>

* <b><u>Scene Save/Load</b>:</u><br><br>
  <img src="src/examples/7_scene.gif"><br><br>
  A Scene can be associated with a `.scene` file:<br><br>
  <img src="src/examples/7_scene_setup.png"><br><br>
  It can then be saved to and loaded back from that file (in-place):<br><br>
  <img src="src/examples/7_scene_update.png"><br><br>
  Saving and loading times are tracked for notification:<br><br>
  <img src="src/examples/7_scene_drawing.png"><br><br>
  <br>

* <b><u>obj2mesh</b>:</u> Also privided is a separate CLI tool for converting `.obj` files to `.mesh` files.<br>
  Usage: `./obj2mesh src.obj trg.mesh [-invert_winding_order] [scale_x:<float>] [rotY:<float>]`<br>
  - invert_winding_order : Reverses the vertex ordering (for objs exported with clockwise order)<br>
  - scale_x:\<float\>: Apply an embedded scaling<br>
  - rotY:\<float\>: Apply an embedded rotation around the Y axis<br>
  
<b>SlimEngine++</b> does not come with any GUI functionality at this point.<br>
Some example apps have an optional HUD that shows additional information.<br>
It can be toggled on or off using the`tab` key.<br>

All examples are interactive, having 2 interaction modes:
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
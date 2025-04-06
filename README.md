# GPUPhysics
OpenGL GPU Spring Mass Model Softbody Simulation
|![teaser](img/softbody2.gif)|
|:--:|
|The GIF may take some time to load|

Description
============
A GPU accelerated 2D softbody physics simulation following the spring mass model. The simulation primarily makes use of Compute Shaders to perform physics calculations on the GPU in order to increase performance and provide a more integrated physics and rendering pipeline.

Known Issues
============
* When running the simulation with very high resolution objects, there is a chance that the point in polygon check will return false positives due to the large number of points which align with the horizontal ray and floating precision errors. In case any of you have an idea to fix this, the point in polygon check occurs in the assets/shaders/compute/physics.glsl compute shader and as of now we are using the raycasting algorithm described in the https://en.wikipedia.org/wiki/Point_in_polygon article while also following the solution of only counting edges which have a point lying below our ray intersection point to limit edge cases which still persist due to floating point precision.
  
Instructions
============
* **WASD** to move the camera around
* **SCROLL** to zoom in and out

Usage
=====
Build and Run the Project inside of Visual Studio. This project was built inside of Visual Studio 2019 Community Version.

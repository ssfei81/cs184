1. Sun Shuo (cs184-ds) and Gabriel Tan (cs184-ci)
2. Windows (Microsoft Visual Studio professional 2012)
3. Code implemented a windows 8 computer and tested on another windows 7 computer

Required Features

1) Render arbitrarily oriented ellipsoids

We implemented a sphere class, and we are able to perform transformations on them to render ellipsoids oriented in 3D space

2) Render polygons

We implemented a triangle class, which serves as the basic building block for all other polygons. We also incorporated loadscene.cpp into our main.cpp, which allows us to read from object files

3) Use simple Phong Shading (in color), Compute shadows, Compute reflections

Our Ray Tracer shades our objects, using formulas for simple Phong shading. If light is blocked, we shade black, thus forming a shadow. Reflections are computed recursively up to a maximum depth (as specified by the user)

* Reflections must be specified in the object file using "reflection R G B", where R, G, and B are floats from 0 to 1. It applies to ALL objects that are rendered after this, so to remove reflection, simply use "reflection 0 0 0"

4) Apply linear transformations to objects

We have a matrix class that creates transformation matrices used to construct ObjectToWorld and WorldToObject matrices for linear transformations in 3D space

5) Use point and directional lights

Similar implementation to our assignment 1

6) Write its output to a standard image format

We incorporated the CImg library, which allows us to export to a .bmp or .ppm file as specified by the user


Extra Credit Implementations

1) Accelerating Ray Tests

We implemented the Axis Aligned Bounding Box algorithm to allow us to compute ray intersections at a much higher speed. It originally took us 6 hours to render the dragon, but after implementing this, it took just a mere 2-3 minutes 

2) Anti-aliasing

We implemented a simple anti-aliasing algorithm by averaging out the color values of neighboring pixels. Add "antiAliasing" to the object file to activate this feature

3) transparency (partially working)
This can be done by specifying the refractive index of the objects by using the command: rindex n where n is the refractive index. All non-dielectrics' rindex must be set to 0.

Misc

1) Input and Output

We allow users to type in your input object files. We also allow users to name their output image files by adding "output XXXXX.bmp" in the object file
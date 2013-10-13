1. Sun Shuo (cs184-ds) and Gabriel Tan (cs184-ci)
2. Windows (Microsoft Visual Studio professional 2012)
3. Code implemented a windows 8 computer and tested on another windows 7 computer

Extra Credit Implementations

1) Anisotropic Diffuse (-an U V)

We applied the formula for Anisotropic Diffusion but could not get the exact effect. The pictures shown on the website are the best effects we found after experimenting with various values of U and V

2) Writing to image file (-image)

We imported a library (Cimg.h) that writes the image into a .bmp file. 

3) "Toon" Shading (-toon)

We assigned discrete values to the colors depending on the intensity of the light falling onto the sphere. 

4) Other Shapes - Torus (-torus)

By applying the formula of the torus, we managed to calculate the normal vector of a torus. This allows us to shade the torus just like a sphere. 

5) Multiple Spheres (just append the conditions of the additional spheres: -ka r g b -kd r g b -ks r g b -sp X max 3 spheres)

6) Shadow (-shadow)

Again, by applying the formula given in the book, we managed to mimic the blocking of light by spheres in front. 

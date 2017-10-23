# npr-renderer

This project is an implementation of Aaron Hertzman's multilayer painterly
rendering algorithm. The algorithm consists of applying a series of brush 
strokes of different sizes and applying them from largest to smallest. My 
submission just uses circles as brush strokes, each with a diameter of 2 to the
power of the layer being used (i.e. {16, 8, 4, 2} for four layers). The 
algorithm returns a list of strokes for each size. An iteration of the 
algorithm will create a reference image that blurs the source image in the 
amount proportional to the current size brush stroke (larger brush strokes use
a reference image that is blurred more than those of smaller brush strokes). 
The reference image is used to calculate areas of the greatest difference in 
the current progress of the painting. These areas are then covered with a stroke
of the current size and painted on the canvas. 

The methods I used to implement this algorithm mainly revolved around simple 
image processing and Qt libraries and functions. I originally tried to use 
Frame Buffer Objects to create images of the current state of the canvas but 
ran into a lot of trouble. I ended up using Pixel Maps and the QPainter class
to draw the strokes of the current brush size and save the image for the next
iteration of the algorithm. This is mainly implemented in the function 
"paintImage()", which uses the helper function "getLayer()" to create a list
of brush strokes of the current size. "getLayer" also creates the blurred 
reference image for the current iteration using the Qt QGraphicsBlurEffect
class. I had originally used the vertex and fragment shaders to implement a 
Gaussian blur but again ran into trouble as I needed to reference the pixels
for the painting function. The "getLayer" computes the source of the greatest 
error within a region defined by the size of the current brush size and adds
a brush stroke only if the error is greater than the threshold determined by 
"m_threshold". The error is defined as the Euclidean distance of the color of
the reference image compared to the corresponding pixel on the current canvas. 
The list of brush strokes are then shuffled at the end of the function to create
a look that corresponds more closely to a painting. Some improvements I would 
make if I had more time would primarily revolve around speedup and 
implementation of a stroke class. The algorithm computing the location of 
strokes of a given region could easily be parallelized using CUDA, as certain 
sections of the image are being considered independently. The way I would 
improve the implementation of the brush stroke would be to use a line 
corresponding to the gradient of a certain region, defined by the greatest 
error within the region being considered. This would improve the "painterly" 
look of the output as the stokes would more closesly resemble real brush stokes.
Also, as of now (December 15), I have a lot of memory that I have not cleaned up
but will try to tomorrow before presenting.

The only resources I used were various Qt libraries and functions. These mainly
inclued QPainter, QPixmap, QGraphicsBlurEffect, along with the textures and 
buffers that were commonly used in class. I also used some of the classes we
used in previous lab assignments, namely the "drawable" class from Lab 2. The 
algorithm I used to paint the layers were taken from Aaron Hertzman's 1998 paper
called "Painterly Rendering with Curved Brush Strokes of Multiple Sizes". Other
than that, no other third party sources were used. 

Compiling the program is simply done by running the executable named "final" in
the build directory. You will need some external image files, however I have
included I few in the directory named "images". When the application is run, 
a blank image will appear on the screen. To open a new image, click on the 
"open" button and select an image file. This will just display the image to the
widget. To run the painting algorithm on the image, select the "paint" button. 
This will run the painting algorithm using the set number of layers and 
error threshold. The program I have submitted uses a threshold of 30 and six 
different brush sizes. There is also a private member variable named 
"m_blurFactor" that multiplies the amount of blur in the reference image by its
given value. A lower blur factor will give a more realistic-looking image. 
The threshold and the blur factor can be modified in the constructor of the 
"painter.cpp" file, and the number of layers can be set in the "paint()" 
function (also in the "painter.cpp" file) near the bottom using the parameters
to the "paintImage" function. These parameters allow you to select the number
of layers to use and whether to stop at a certain size brush. These functions
are the main features of the program. The rest of the program simply
assigns the "m_texture" variable to a QImage and outputs the image to the screen
by mapping to a rectangle and sampling the image using the vertex and fragment
shaders. 

While the implementation of the algorithm is fairly straightforward, this took
more time than I thought to set up and become functional. I also implemented 
various parts of the program in multiple different ways before settling on what
I have turned in. This project has definitely given me a greater understanding
of Modern OpenGL, Qt, and the general foundation of our labs throughout the 
semester. I hope that I can continue to work on this project and make the 
improvements suggested above. 

## References
 
 http://cs.brown.edu/people/bjm/painterly.pdf

 https://www.cs.duke.edu/courses/cps124/spring04/notes/13_npr/cmu.pdf

 https://www.cs.virginia.edu/~luebke/publications/pdf/npr.pdf
 
 

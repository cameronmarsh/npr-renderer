So far I have connected my program to the shaders and implemented a simple
single-pass Gaussian blur in the fragment shader. Setting up the shaders 
proved to be the more challenging aspect of the project so far, so I am 
a bit behind on my goals laid out in my proposal. However, the rest of my 
project should be relatively simple now that everything is connected and 
working well. The only thing I need to fix in terms of setting up the 
application is the open() slot, it is not currently opening the new image when
I select it. 

Moving forward, I need to figure out how to implement a brush stroke using 
a cubic spline function. I also need to figure out how to control the amount of
blur the image is given prior to rendering the brush strokes. I think I am 
going to implement a frame buffer object to pass the same image through the 
blur filter multiple times, and then render brush strokes a number of times
to control the detail within the painting. I have multiple resources providing 
guidance on how to do this layered-rendering, so I think that I will have 
plenty of time to finish before presenting. 

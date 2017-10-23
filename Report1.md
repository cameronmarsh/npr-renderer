This week I hoped to set up the shaders and implement a blurring of an input
texture. I think I underestimated how little I knew about shaders and passing
textures in to process, so I am a little behind on my goals. I have written
functions in the painter.cpp file to pass in the texture data onto a sqaure
that takes up the space on the screen but am still working on the blurring
effect. I have multiple resources available to me, and have a good idea of 
how I will implement the blurring filter. I plan on using a Frame Buffer Object
to make multiple passes through the image, performing a Gaussian blurring
(or maybe linear sampling) on each iteration. 

I think the only adjustments I need to make to my proposal is the deadlines
that I set for myself. Once I figure out the setup of the shaders and processing
images correctly, I should have an easier time implementing the brush strokes
and rendering the npr output. 

# Interaction

Note: this only contains files that were in the "01_Pipeline_Interaction" folder in the original OpenGL template. Other files included in the template will have to be manually added in as the filesize and number of files in the rest of the template was too high to consider committing to this repository.

# Importing

To export my Maya objects to OpenGL, I initially exported each model within the scene as .obj files. However, when initially attempting to import objects into OpenGL, I immediately ran into problems as whilst some meshes could load in fine most meshes returned an "Expression: vector subscript out of range" error.

![test](/Appendices/DevelopmentImages/Interaction/Importing/Screenshot 2021-11-19 140940.png) *Error shown when I attempted importing on most models from my Maya project*

I attempted to fix this within Maya, however when exporting .obj files in Maya the options for exporting .obj files are limited so I couldn't alter the exported .obj files much. I looked online for solutions and adjusted my models in the Maya project but none resolved the problem satisfactorily.

I went onto the Simple OpenGL tutorial linked in the Canvas course modules and noticed that models there were being exported with Blender, so I attempted that and adjusted Blender's export settings accordingly for the original exported .obj files from Maya and I got my models to import successfully (sidenote: I know we shouldn't be using Blender however it was only used to re-export .obj files and adjust positioning for import transformations, nothing else).

![test](/Appendices/DevelopmentImages/Interaction/Importing/Screenshot 2021-11-19 140159.png) *Blender export settings used to successfully export models to OpenGL*


![test](/Appendices/DevelopmentImages/Interaction/Importing/Screenshot 2021-11-19 140059.png) *Successful import of a model that failed with the earlier error*

.mtl files were also exported alongside .obj files, and were unaltered. Some .obj files received very slight edits to link them to their respective material files. 

Initially, objects were importing well, but in the wrong position which caused several interactivity functions like model rotation to be very off.

![test](/Appendices/DevelopmentImages/Interaction/Importing/Screenshot 2021-11-19 165603.png) 

*Example of objects not positioned properly*

To resolve this, I went back into Blender and repositioned objects accordingly so that everything would be in the correct position after being centered by model position code in the Source.cpp file and would function properly with interactive functions.

![test](/Appendices/DevelopmentImages/Interaction/Importing/Screenshot 2021-11-19 171738.png) *Adjusted model placements in Blender*

Afterwards, the objects were all imported properly and didn't rotate, move or scale weirdly when moving the models. To further ensure the positions remained stable I gave each model a position variable to store that models position in the OpenGL application.

![test](/Appendices/DevelopmentImages/Interaction/Importing/Screenshot 2021-11-19 173228.png) *All objects imported properly (except the witch but that object wasn't imported at the time when taking the screenshot - it imported properly too)*

# Materials, Lighting and Shaders

Regrettably, I couldn't successfully implement texturing and updated shaders in the OpenGL application. I attempted numerous types of implementation to ensure they were working, but nothing worked and I had to cut my losses and move on to implementing interactivity in the app. I did implement a lighting system but without reworked shaders it's useless. I also seemed to be able to load in a texture for the dirt path at one point however again, without working shaders it was useless. The code for these implementations still remains in the Source.cpp file. 

The shaders used ended up just being the template provided shaders as my attempts to implement them didn't work, leading to errors like this:

![test](/Appendices/DevelopmentImages/Interaction/Interactivity/error.png) 

*App loaded in with shaders but wasn't functional and spammed this error which I couldn't fix*

![test](/Appendices/DevelopmentImages/Interaction/Interactivity/Screenshot 2021-11-22 000615.png) 

*Another error after an attempted fix by altering glCreateProgram and glLinkProgram placements*

# Interactivity

Thankfully, implementing interactivity functions in the application was much more straightforward for the most part.

The most troublesome part was implementing windmill blade rotation. Beforehand, I separated the windmill blades and base to split them into different models so that the blades could rotate in the animation portion of the coursework so I reused those models here. Beforehand, I created a "windmillbladesRotation" variable to allow the blades to be rotated easily, however it initially didn't work out well in the application.

![test](/Appendices/DevelopmentImages/Interaction/Interactivity/Screenshot 2021-11-20 215152.png)

*Windmill blades detaching from the windmill and rotating erratically*

To fix this, I attempted to alter the positions of the windmill blades and windmill base in OpenGL to no avail. Eventually I figured out 2 key things: repositioning the blades in Blender so that the pole connecting the blades was on the desired axis and adding windmillbladesRotation to the modelMatrix parts before the windmill blades were drawn so that rotation would be smoother. I programmed the rotation for the blades to rotate them a certain amount on the x axis when the **5** was pressed, alongside the **6** key that rotated the blades the other way on the x-axis.

![test](/Appendices/DevelopmentImages/Interaction/Interactivity/Screenshot 2021-11-21 004520.png)

![test](/Appendices/DevelopmentImages/Interaction/Interactivity/Screenshot 2021-11-21 014846.png)

*Windmill blades and base being aligned normally and blades rotating as intended when pressing a key on the keyboard*

Everything else went smoothly in regards to implementation. One interactive component I made involved pressing the **O** and **P** keys to slide the house door across the x-axis up to a certain point so the door didn't go flying through the house. This was done by using an if statement to only allow the object to move up to a certain position.

I also let the scene be deconstructed + reconstructed by having almost every object move a certain direction away from the center of the object when the **8** key was held, and then when the **9** key was pressed the objects  gradually return to their original position, going further if the key's still held.

![test](/Appendices/DevelopmentImages/Interaction/Interactivity/Screenshot 2021-11-21 020603.png)

*Deconstructed scene after key presses move the objects from the center*

A camera system was also implemented where you use the **WASD** keys to move it around the scene without moving the model, and more minor interactions were added as follows:

- Island moves down the y axis when pressing the **4** key
- Rocks rotate around the y axis when pressing the **7** key
- The bench moves in a positive direction on the x axis when the **3** key is held
- The rocks move up the y axis when the **2** key is held

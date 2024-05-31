# Animation

The Unreal project doesn't include the DerivedDataCache, DerivedDataCache/VT and Script folders, however all these folders were empty upon finishing up with Unreal.

# Workflow

I initially exported my scene as an .fbx file, but ran into these errors:

![Error](/Appendices/DevelopmentImages/Animation/Errors/Screenshot 2021-11-16 120957.png)*Initial errors upon importing everything for the first time*

To resolve this, I enabled Smoothing Groups in the Maya export window and exported each object individually as an .fbx file before importing them into Unreal.

![Error](/Appendices/DevelopmentImages/Animation/Errors/Screenshot 2021-11-16 142610.png)*Island successfully imported*

Once each object had been imported, this is what it looked like:

![Error](/Appendices/DevelopmentImages/Animation/Errors/Screenshot 2021-11-16 150507.png)*Initial state of objects after importing them to Unreal*

Not too bad however the lighting was messed up and needed rebuilt, and some materials looked brighter, darker or simply non-existent in the case of the ponds border. After this I built the scene and lighting and a lot of issues were discovered in the scene:

![Error](/Appendices/DevelopmentImages/Animation/Errors/Screenshot 2021-11-16 170708.png)*After lighting was built (sky light was temporarily changed here too)*

I discovered a lot of clipping on the house object which most likely caused the very wonky lighting on it, and in addition to that some objects either had wonky lighting too like the pond or their materials turned into a black colour.

To fix these issues, I started with the house. I went back into Maya and fixed clipping issues on the sides of the house and then discovered that the "window glass" (black planes) weren't showing in Unreal so I went and redid the planes for each window to ensure that they showed up in Unreal.In addition, I increased the light map resolution in order to prevent the lighting from being affected by any other issues with the house:

![Error](/Appendices/DevelopmentImages/Animation/Errors/Screenshot 2021-11-16 183242.png)*Light map resolution bumped up a lot from 64 to 512, making the lighting normal as a result*

There was still the issue of the stone walls on it being too dark, so I went into the texture editor and increased the brightness of the stone texture I was using to get an appearance more in line with my references and Maya scene:

![Error](/Appendices/DevelopmentImages/Animation/Errors/Screenshot 2021-11-16 183814.png)*Fixed stone texture via increasing its brightness, making the house more normal*

For the other material issues, I noticed that by changing the material property to two-sided for the materials that showed as black, they then looked normal upon the next rebuild of lighting. I also altered the material used for the pond water slightly to give it a more realistic and animated water appearance.

I also imported the witch object and following up on my rig of it in Maya, I attempted to pose it but I wasn't able to do so, so I left it T-posing for the rest of the animation phase of the project. Other than that, I had managed to import each object successfully and the scene was ready for animation by my standards:

![Error](/Appendices/DevelopmentImages/Animation/Errors/Screenshot 2021-11-16 203116.png)*Scene with fixed objects and lighting*

# Animation

For the animation, I wanted the witch to walk to the house and enter it whilst using magic to manipulate nearby objects on the way. Of course, I couldn't get the witch's animation to work so I just made her hover to inside the house instead.

Before animation could proceed I wanted the house door and windmill blades to rotate around the appropriate axes, so I split the door and windmill blades from their respective models and reimported them from Maya, however the rotation for both was erratic. With this in mind, I adjusted their positions in Maya so that they would have proper rotations, and upon reimporting them they rotated as I expected.

To begin my animation, I set up the MasterSequencer object to create my sequence and split my viewport into two for a better workflow.

![Error](/Appendices/DevelopmentImages/Animation/Animation/Screenshot 2021-11-17 145348.png)*Image of my setup for animating my scene*

On the left viewport, I moved and altered objects like normal, meanwhile on the right viewport I created a Camera actor and locked it to that viewport, so even if I move the normal camera on the left viewport the right viewport maintains the same perspective, allowing me to easily handle positioning for cameras and objects during the animation.

![Error](/Appendices/DevelopmentImages/Animation/Animation/Screenshot 2021-11-18 132039.png)*Image during the process where the camera actor was in a poor position so I had to adjust it to follow the witch properly*

In regard to what I did to make the animation more interesting movement wise, I set a straight path for the witch to follow to inside the house, but to make it more interesting I made the witch quickly hover up onto the island and knock some rocks off of the island:

![Error](/Appendices/DevelopmentImages/Animation/Animation/Screenshot 2021-11-18 140318.png)*Transformation process of rocks falling off the island with a blue light to make it flashier and thematically appropriate*

I also had the windmill blades spin after a quick flash of pink light before shifting the camera towards the pond and giving it a red light that slowly faded in alongside 3 rocks spinning in a circle above it. Finally, the witch entered the house as I set the camera to face straight towards the house before quickly going to a diagonal view with the grave in view. For a more interesting finale, I quickly had the camera zoom to the grave and had a different coloured witch rise out of it, with a green light emanating from her. I decided to employ a fade track here too so that the animation would fade to black, creating a more ominous ending in the process.

I employed an abundance of point lights for magical effects, with some being flashes fading in and out very quickly and others lingering more like the green light and the door light. A slightly more detailed explanation for these lights can be found in the Appendices.

![Error](/Appendices/DevelopmentImages/Animation/Animation/Screenshot 2021-11-18 133907.png)

![Error](/Appendices/DevelopmentImages/Animation/Animation/Screenshot 2021-11-18 143602.png)

![Error](/Appendices/DevelopmentImages/Animation/Animation/Screenshot 2021-11-18 144405.png)

*Render settings for the animation - see the appendix for an explanation*

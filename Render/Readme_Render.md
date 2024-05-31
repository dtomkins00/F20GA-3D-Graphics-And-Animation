# Render

Note regarding Render.png: for the best quality of the render open up the image in a new tab and zoom in appropriately

I have a bit of experience with making models and renders in Blender so I'm familar with how modelling software works and how it functions, so Maya wasn't too bad to adjust to and use it's more complex tools. I will only go over more some more unique/involved parts of the modelling process for certain objects - simple things like basic extrusion and normal vertice movement won't be covered but you can look at the images in the Render folder in the Development Images folder in the Appendices for more evidence. If anything needs further explanation it will be covered in the appendices. Some of these images are remodels of the original model as I forgot to screenshot development for a bunch of them at first so I remade them to show the process.

# Bench

![test](/Appendices/DevelopmentImages/Render/Bench/Screenshot 2021-11-23 185736.png) *Edge loops to create smoothness on the sides and front of the bench arm*

![test](/Appendices/DevelopmentImages/Render/Bench/Screenshot 2021-11-23 191625.png) *Edge loops to create smoothness on the top of the bench side with soft selection used to pull the top up appropriately*

![test](/Appendices/DevelopmentImages/Render/Bench/Screenshot 2021-11-23 193830.png) *Shift+D duplication to have accurate spacing on the back of the bench's planks*

![test](/Appendices/DevelopmentImages/Render/Bench/Screenshot 2021-11-23 195221.png) *Beveled cuboid for smoothness and to serve as foundation for seats*

![test](/Appendices/DevelopmentImages/Render/Bench/Screenshot 2021-11-23 195735.png) *Duplicated + rotated above cuboid a lot to create the bench seat*

# House

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 153649.png) *Set subdivisions of a polygonal plane to 1 for width and 2 for height, then scaled it accordingly using edges and extruded its faces to create a basic roof*

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 155615.png) *Created roof tile by moving 2 faces on a scaled cube*

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 160923.png) *After Shift+D'ing roof tiles and adjusting them to look like they're placed underneath each other, I rotated a tile at the edge and cut it in half to fill up the row of tiles without going off the roof*

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 161351.png) *Tiles laid over each other and aligned on the roof*

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 164401.png) *To create a curved window I used a bend deformer to bend a cuboid to fit the window size by gradually adjusting the bend handles to get the desired bend direction and curvature to get the correct shape*

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 165437.png) *I used the multicut tool to carve a hole in the house to insert space for a door + door frame*

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 172223.png) *For the chimney brick I made a block and gave it 2 edge loops each way before smoothing the block to get a smooth block*

![test](/Appendices/DevelopmentImages/Render/House/Screenshot 2021-11-23 172457.png) *For a better looking chimney I adjusted brick locations to give the chimney a more natural look with the differently orientated placements*

# Flowers

![test](/Appendices/DevelopmentImages/Render/Flower/Screenshot 2021-11-23 140417.png) 

*Initial position of vertices on a polygonal plane for flower petals - done by creating edge loops and adjusting them to be in the middle of their sections for a smoother looking petal*

![test](/Appendices/DevelopmentImages/Render/Flower/Screenshot 2021-11-23 141937.png) 

*After adjusting vertice positions and smoothing petal, final petal result*

![test](/Appendices/DevelopmentImages/Render/Flower/Screenshot 2021-11-23 143014.png) 

*Scaled a sphere accordingly, gave it subdivisions of 25 height + width and manually selected every other vertex up to a certain point before moving them down - this was to create a more realistic looking flower bud*

![test](/Appendices/DevelopmentImages/Render/Flower/Screenshot 2021-11-23 144234.png) 

*Adjusted pivot point of petal to the center, duplicated and rotated it then Shift+D'd to cover the flower bud. Then selected all petals and rotated+moved them slightly to fill the space up more*

![test](/Appendices/DevelopmentImages/Render/Flower/Screenshot 2021-11-23 145309.png) 

*Halfed a sphere, selected every other edge inside it and moved them inwards to create a base for the flower bud before smoothing it*

# Witch

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-12 234526.png) 

*On 2 faces I used Extrude and created a large offset before moving the faces outwards and scaling them appropriately to create a suitable thumb size. Then I extruded the face(s) on top to create a thumb for the hand*

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-13 000645.png) 

*Added edge loops to the elbow area and adjusted vertices to make for smoother movement during animation*

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-13 002726.png) 

*Used multi-cut tool to carve an area for the neck to be extruded from - should also mention the body is mirrored in a way so that changes I make to one half affect the other, allowing for better symmetry and accuracy in the model*

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-13 195424.png) 

*After creating head, seams were split so I had to snap vertices together to prevent any gaps showing in the model*

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-13 195424.png) 

*Adjusted pivot point of petal to the center, duplicated and rotated it then Shift+D'd to cover the flower bud. Then selected all petals and rotated+moved them slightly to fill the space up more*

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-14 113429.png) 

*Started modelling clothing and hair for the witch by extruding faces on the head to create hair and then body faces to create a dress (in hindsight these should have been modeled separately from the witch as it messed up my UV unwrapping)*

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-14 163742.png) 

*After clothing was created, I used Quick Rig and adjusted the guides to successfully create a rig for my witch which was supposed to be used in the animation*

**Further modelling explanations can be found in the appendices.**

# Render Settings

![test](/Appendices/DevelopmentImages/Render/Character/Screenshot 2021-11-15 202933.png) 

*To get a good quality render, I increased Camera(AA) samples to 8 for higher quality output without going overboard, Diffuse at 3 to reduce noise from rays, Specular at 4 for better reflections (particularly the pond), Transmission to 4 for further noise reduction, SSS to 3 for cleaner lighting in my scene and Volume Indirect to 4 for more natural lighting and noise reduction. These settings are like this to output a great quality image whilst keeping render times reasonable by reducing the amount of calculations needed to account for each sampling aspect. Image size was also set to 1920x1080 image size and given a resolution of 300 dpi to ensure the rendered image was high quality.*

*I positioned the camera so that it was a good distance from the scene - this allowed me to capture the whole scene whilst keeping the background in the scene. The camera was given a diagonal overhead view to emphasise the larger buildings and portray the lighting better, whilst providing the models in the scene with a nicer angle to look at in combination with the lighting. A blue curved plane was used to create a sky-themed background to serve as a backdrop to the scene.*

**See appendices for further explanations.**

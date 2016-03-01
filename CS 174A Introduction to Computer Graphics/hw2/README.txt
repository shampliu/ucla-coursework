I chose to do a Star Wars themed animation of the BB-8 robot fleeing from an enemy droid pursuer. BB-8 eventually bumps into a hooded figure who turns out to be a jedi. If I had more time on this, I would have added a scene where the Jedi fights the droid. I also would have added sound effects and subtitles.

The BB-8 model was a hierarchical model. I made a global variable to reference the object and separated the types of references by body part. One property, called "all", would modify the whole model while the variables "head" and "body" separated the parts and modified them individually. This way I was able to make BB-8 turn its head frantically as it fleed its pursuer. 

Most of the complex objects - the Jedi cloak, lightsaber, BB-8's head, the droid head - were modeled in Blender and exported as triangulated OBJ files to be imported into WebGL. I also wrote code to draw a triangulated plane in the shapes.js file. This plane was versatile because I added a parameter in the function to modify the height, so I could create pyramidal shapes in addition to the flat surfaces. The normals were handled by the pre-written function and this object is successfully flat shaded.

Regarding texture, I created my own design in Adobe Illustrator for BB-8's body. It was a square 512 x 512 pixel PNG and when I applied the texture map to the spherical body, it mapped very smoothly. 




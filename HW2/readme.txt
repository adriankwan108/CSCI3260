CSCI3260 Assignment 2 Texture Mapping/ Lighting Control  

Manipulation:

	View Control:
	Mouse:  Click "Left" button and the mouse MOVES UP, scene you see moves down.
	        meanwhile, click "left" button and your mouse MOVES LEFT, scene you see moves right.

		Wheel UP zooms in to the centre (0,0,0);
		Wheel Down zooms out from the the centre.


	Animation Control:
	Keyboard:  Press "↑↓←→" can control the cat's movement on the ground,
		   which indicates forward, backward movement, and left, right rotation respectively,
		   from the view of the cat.


	Texture Control:
	Keyboard:  Press "1" to "2" to switch the texture for the cat.


	Lighting Control:

	I have used two light sources,
	one is the directional light from the right hand side of the scene.
	The other one is a spotlight, fixed and emitting from the top of centre to the ground, 
		the spotlight direction will rotate around y-axis, red in color.


	Keyboard:  Press "w" and "s" to increase and reduce the brightness of directional light.
		   Press "p" to pause or continue the rotation.
	
	The brightness of directional light will not affect the brightness of spotlight.
	Thus, we can see the effect after the combination of directional light and the spotlight,
		or close the directional light (lower the brightness, press "s" until total darkness)
		and see the spotlight effect only.

	i.e. the diffuse light effect and specular light effect under spotlight / spotlight + directional.

	
	Bonus:
	
	1.Using spotlight.

	2.Interesting effect and scenes.

	  There is a meaningful scene after the directional light is turned off.
 	  You need to press "s" until total darkness.
	  Under total darkness, the cat OUTSIDES the spotlight will not have 
	  any diffuse nor specular effect from directional light.

	  Then, the spotlight will turn on a "flicker mode", flash light is resulted,
	  forming a creepy terror atmosphere due to the black flashing-red environment.

	3.The cat cannot moves outside the floor.
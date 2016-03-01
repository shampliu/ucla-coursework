// *******************************************************
// CS 174a Graphics Example Code
// animation.js - The main file and program start point.  The class definition here describes how to display an Animation and how it will react to key and mouse input.  Right now it has 
// very little in it - you will fill it in with all your shape drawing calls and any extra key / mouse controls.  

// Now go down to display() to see where the sample shapes are drawn, and to see where to fill in your own code.

"use strict"
var canvas, canvas_size, gl = null, g_addrs,
	movement = vec2(),	thrust = vec3(), 	looking = false, prev_time = 0, animate = false, animation_time = 0;
		var gouraud = false, color_normals = false, solid = false;
function CURRENT_BASIS_IS_WORTH_SHOWING(self, model_transform) { self.m_axis.draw( self.basis_id++, self.graphicsState, model_transform, new Material( vec4( .8,.3,.8,1 ), 1, 1, 1, 40, "" ) ); }



// *******************************************************	
// When the web page's window loads it creates an "Animation" object.  It registers itself as a displayable object to our other class "GL_Context" -- which OpenGL is told to call upon every time a
// draw / keyboard / mouse event happens.

window.onload = function init() {	var anim = new Animation();	}
function Animation()
{
	( function init (self) 
	{
		self.context = new GL_Context( "gl-canvas" );
		self.context.register_display_object( self );
		
		gl.clearColor( 245/255,208/255,182/255, 1 );			// Background color

		self.m_cube = new cube();
		self.m_axis = new axis();
		self.m_sphere = new sphere( mat4(), 4 );	
		self.m_fan = new triangle_fan_full( 10, mat4() );
		self.m_strip = new rectangular_strip( 1, mat4() );
		self.m_cylinder = new cylindrical_strip( 10, mat4() );

		self.m_triangle = new triangle( mat4() );

		// BB-8 OBJECTS
		self.m_top_half = new shape_from_file( "top.obj" );
		self.m_bottom_half = new shape_from_file( "bottom.obj" );

		// DROID OBJECTS
		self.m_droid_head1 = new shape_from_file( "droid-head-1.obj" );
		self.m_droid_head2 = new shape_from_file( "droid-head-2.obj" );
		self.m_droid_head3 = new shape_from_file( "droid-head-3.obj" );
		self.m_droid_head4 = new shape_from_file( "droid-head-4.obj" );

		self.m_droid_body = new shape_from_file( "droid-body.obj" );

		self.m_droid_leg1 = new shape_from_file( "droid-leg-1.obj" );
		self.m_droid_leg3 = new shape_from_file( "droid-leg-3.obj" );

		self.m_droid_blaster = new shape_from_file( "droid-blaster.obj" );

		// MATERIAL'D STUFF
		self.m_laser = new shape_from_file( "laser.obj" );

		// JEDI OBJECTS
		self.m_jedi_cloak = new shape_from_file( "jedi-cloak.obj" );
		self.m_jedi_head = new shape_from_file( "jedi-head.obj" );
		self.m_jedi_nose = new shape_from_file( "jedi-nose.obj" );
		self.m_jedi_body = new shape_from_file( "jedi-body.obj" );
		self.m_jedi_leg = new shape_from_file( "jedi-leg.obj" );
		self.m_jedi_lightsaber = new shape_from_file( "jedi-lightsaber.obj" );

		self.m_plane = new plane( mat4(), 0 );
		self.m_hill = new plane( mat4(), 1/80 );

		self.global_bb = { };
		self.global_bb.all = mat4();
		self.global_bb.body = mat4();
		self.global_bb.head = mat4();

		self.global_droid = { };
		self.global_droid.all = mat4();
		self.global_droid.offsetX = 0;

		self.global_laser1 = {};
		self.global_laser1.all = mat4();
		self.global_laser1.fired = false; 

		self.global_laser2 = {};
		self.global_laser2.all = mat4();
		self.global_laser2.fired = false; 

		self.global_jedi = {};
		self.global_jedi.all = mat4();

		// 1st parameter is camera matrix.  2nd parameter is the projection:  The matrix that determines how depth is treated.  It projects 3D points onto a plane.
		self.graphicsState = new GraphicsState( translate(-140, -20, -200), perspective(45, canvas.width/canvas.height, .1, 1000), 0 );

		gl.uniform1i( g_addrs.GOURAUD_loc, gouraud);		gl.uniform1i( g_addrs.COLOR_NORMALS_loc, color_normals);		gl.uniform1i( g_addrs.SOLID_loc, solid);
		


		self.context.render();	

	} ) ( this );	
	
	canvas.addEventListener('mousemove', function(e)	{		e = e || window.event;		movement = vec2( e.clientX - canvas.width/2, e.clientY - canvas.height/2, 0);	});
}

// *******************************************************	
// init_keys():  Define any extra keyboard shortcuts here
Animation.prototype.init_keys = function()
{
	shortcut.add( "Space", function() { thrust[1] = -1; } );			shortcut.add( "Space", function() { thrust[1] =  0; }, {'type':'keyup'} );
	shortcut.add( "z",     function() { thrust[1] =  1; } );			shortcut.add( "z",     function() { thrust[1] =  0; }, {'type':'keyup'} );
	shortcut.add( "w",     function() { thrust[2] =  1; } );			shortcut.add( "w",     function() { thrust[2] =  0; }, {'type':'keyup'} );
	shortcut.add( "a",     function() { thrust[0] =  1; } );			shortcut.add( "a",     function() { thrust[0] =  0; }, {'type':'keyup'} );
	shortcut.add( "s",     function() { thrust[2] = -1; } );			shortcut.add( "s",     function() { thrust[2] =  0; }, {'type':'keyup'} );
	shortcut.add( "d",     function() { thrust[0] = -1; } );			shortcut.add( "d",     function() { thrust[0] =  0; }, {'type':'keyup'} );
	shortcut.add( "f",     function() { looking = !looking; } );
	shortcut.add( ",",     ( function(self) { return function() { self.graphicsState.camera_transform = mult( rotate( 3, 0, 0,  1 ), self.graphicsState.camera_transform ); }; } ) (this) ) ;
	shortcut.add( ".",     ( function(self) { return function() { self.graphicsState.camera_transform = mult( rotate( 3, 0, 0, -1 ), self.graphicsState.camera_transform ); }; } ) (this) ) ;

	shortcut.add( "r",     ( function(self) { return function() { self.graphicsState.camera_transform = mat4(); }; } ) (this) );
	shortcut.add( "ALT+s", function() { solid = !solid;					gl.uniform1i( g_addrs.SOLID_loc, solid);	
																		gl.uniform4fv( g_addrs.SOLID_COLOR_loc, vec4(Math.random(), Math.random(), Math.random(), 1) );	 } );
	shortcut.add( "ALT+g", function() { gouraud = !gouraud;				gl.uniform1i( g_addrs.GOURAUD_loc, gouraud);	} );
	shortcut.add( "ALT+n", function() { color_normals = !color_normals;	gl.uniform1i( g_addrs.COLOR_NORMALS_loc, color_normals);	} );
	shortcut.add( "ALT+a", function() { animate = !animate; } );
	
	shortcut.add( "p",     ( function(self) { return function() { self.m_axis.basis_selection++; console.log("Selected Basis: " + self.m_axis.basis_selection ); }; } ) (this) );
	shortcut.add( "m",     ( function(self) { return function() { self.m_axis.basis_selection--; console.log("Selected Basis: " + self.m_axis.basis_selection ); }; } ) (this) );	
}

function update_camera( self, animation_delta_time )
	{
		var leeway = 70, border = 50;
		var degrees_per_frame = .0005 * animation_delta_time;
		var meters_per_frame  = .03 * animation_delta_time;
																					// Determine camera rotation movement first
		var movement_plus  = [ movement[0] + leeway, movement[1] + leeway ];		// movement[] is mouse position relative to canvas center; leeway is a tolerance from the center.
		var movement_minus = [ movement[0] - leeway, movement[1] - leeway ];
		var outside_border = false;
		
		for( var i = 0; i < 2; i++ )
			if ( Math.abs( movement[i] ) > canvas_size[i]/2 - border )	outside_border = true;		// Stop steering if we're on the outer edge of the canvas.

		for( var i = 0; looking && outside_border == false && i < 2; i++ )			// Steer according to "movement" vector, but don't start increasing until outside a leeway window from the center.
		{
			var velocity = ( ( movement_minus[i] > 0 && movement_minus[i] ) || ( movement_plus[i] < 0 && movement_plus[i] ) ) * degrees_per_frame;	// Use movement's quantity unless the &&'s zero it out
			self.graphicsState.camera_transform = mult( rotate( velocity, i, 1-i, 0 ), self.graphicsState.camera_transform );			// On X step, rotate around Y axis, and vice versa.
		}
		self.graphicsState.camera_transform = mult( translate( scale_vec( meters_per_frame, thrust ) ), self.graphicsState.camera_transform );		// Now translation movement of camera, applied in local camera coordinate frame
	}

// *******************************************************	
// display(): called once per frame, whenever OpenGL decides it's time to redraw.

Animation.prototype.display = function(time)
{
	if(!time) time = 0;
	this.animation_delta_time = time - prev_time;
	if(animate) this.graphicsState.animation_time += this.animation_delta_time;
	prev_time = time;
	
	update_camera( this, this.animation_delta_time );
		
	this.basis_id = 0;
	
	var model_transform = mat4();
		
	/**********************************
	Start coding here!!!!
	**********************************/

	var stack = []; 
	stack.push(model_transform);	
											
													
	this.draw_ground(model_transform); 


	var t = this.graphicsState.animation_time - 3000;

	this.global_droid.all = mult( this.global_droid.all, translate( 0, .1 * Math.sin (t / 100), 0 ) )
	


	if (t < 0) {

	}
	else if (t < 3000) {
		this.global_bb.body = mult( this.global_bb.body, rotate( (t/3 * -1), 0, 0, 1 ) );
		this.global_bb.all = mult( this.global_bb.all, translate( 0, 0.14 * (t / 3000), 0.4 * (t / 3000)) )
		this.draw_BB(this.global_bb);
	}
	else if (t < 4500) {
		this.global_bb.body = mult( this.global_bb.body, rotate( (t/3 * -1), 0, 0, 1 ) );
		this.global_bb.all = mult( this.global_bb.all, translate( -1.1 * Math.sin(t/120), -0.06 * (t / 3000), 0.05 * (t / 3000) ) );
		// this.global_bb.head = mult( this.global_bb.head, rotate( -5.5, 0, 1, 0 ) );
		this.global_bb.head = mult( this.global_bb.head, rotate( 7.5 * Math.sin(this.graphicsState.animation_time/200), 1, 0, 0 ) );

		// this.global_bb.head = mult( this.global_bb.head, rotate( ( 30 * Math.cos(t/100)), 0, 1, 0 ) );
		this.draw_BB(this.global_bb);


	}
	else if (t < 12000) {
		if (t < 6000) {
			this.global_bb.all = mult( this.global_bb.all, translate( 0, -0.04 * (t / 3000), 0 ) );
			this.global_droid.all = mult( this.global_droid.all, translate( 0, 0.08 * (t / 4000), 0.05 * (t / 3000)) )
			this.draw_droid(this.global_droid, -5);
			this.global_bb.head = mult( this.global_bb.head, rotate( 6 * Math.sin(this.graphicsState.animation_time/200), 1, 0, 0 ) );

		}
		else if (t < 7000) {
			this.global_droid.all = mult( this.global_droid.all, translate( 0, -0.12 * (t / 4000), 0.05 * (t / 3000)) )
			this.draw_droid(this.global_droid, -5);
			this.global_bb.head = mult( this.global_bb.head, rotate( 6 * Math.sin(this.graphicsState.animation_time/200), 1, 0, 0 ) );

		}
		else if (t < 7500) {
			this.draw_laser(this.global_laser1);
			this.global_bb.head = mult( this.global_bb.head, rotate( 6 * Math.sin(this.graphicsState.animation_time/200), 1, 0, 0 ) );
		}
		else if (t < 8200) {
			this.global_laser1.fired = false;
			this.draw_laser(this.global_laser2);
			this.global_bb.head = mult( this.global_bb.head, rotate( 6 * Math.sin(this.graphicsState.animation_time/200), 1, 0, 0 ) );
		}
		else if (t < 9000) {
			this.draw_laser(this.global_laser1);
			this.global_laser2.fired = false;
		}
		this.global_bb.body = mult( this.global_bb.body, rotate( (t/3 * -1), 0, 0, 1 ) );

		this.global_droid.all = mult( this.global_droid.all, translate( 0, 0, 0.2 * (t / 3000)) )
		this.draw_droid(this.global_droid, -5);
		// this.global_bb.all = mult( this.global_bb.all, translate( 0, 0, 0.4 * (t / 3000) ) );
		if (t < 8000) {
			this.global_bb.all = mult( this.global_bb.all, translate( -1.1 * Math.sin(t/120), 0, 0.4 * (t / 3000) ) );
		}
		else { 
			this.global_bb.all = mult( this.global_bb.all, translate( 0.1 * t/20000, 0, 0.4 * (t / 3000) ) );
			this.global_bb.head = mult( this.global_bb.head, rotate( .6 * Math.sin(this.graphicsState.animation_time/20000), 1, 0, 0 ) );
		}
		
		
		this.draw_BB(this.global_bb);
		this.draw_jedi(this.global_jedi);

	}
	else if (t < 30000) {
		if (t < 16000) {
			this.global_droid.all = mult( this.global_droid.all, translate( 0, 0, 0.2 * (t / 3000)) )
			this.draw_droid(this.global_droid, -5);
			this.global_bb.head = mult( this.global_bb.head, rotate( -.15 * Math.sin(this.graphicsState.animation_time/20000), 0, 1, 0 ) );

		}
		this.draw_BB(this.global_bb);
		this.draw_jedi(this.global_jedi);

	}

	
	
	

	// this.graphicsState.camera_transform = lookAt( vec3(0,0,5), vec3(0,0,-1), vec3(0,1,0) );
	
}

Animation.prototype.draw_laser = function(laser) {
	var red = new Material( vec4( 0.9, 0, 0 ), .5, 1, 1, 10 );
	if (!laser.fired) {
		laser.all[0][3] = this.global_droid.offsetX;  // fix this 
		laser.all[1][3] = this.global_droid.all[1][3] + 5; // account for height 
		laser.all[2][3] = this.global_droid.all[2][3]; 

		// laser.all = mult( laser.all, rotate( theta, 0, 1, 0 )) ;

		laser.fired = true; 
	}
	
	laser.all = mult( laser.all, translate( 0, 0, 5 ) );
	
	this.m_laser.draw( this.graphicsState, laser.all, red )
}

Animation.prototype.draw_ground = function(model_transform) {
	var stack = [];
	stack.push(model_transform);

	var ground_color = new Material( vec4( 246/255, 180/255, 102/255 ), 1, 1, 1, 40 );
	// var t = new Material( vec4( .5,.5,.5,1 ), 1, 1, 1, 40, "sand.png" );
	// var back_color = new Material( vec4( 189/255, 138/255, 78/255 ), 1, 1, 1, 40 );

	model_transform = mult( model_transform, translate( 55, 0, 0 ) );

	this.m_hill.draw( this.graphicsState, model_transform, ground_color );	

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( -200, 0, 0 ) );
	model_transform = mult( model_transform, scale( 100, 1, 100 ) ); 

	this.m_plane.draw( this.graphicsState, model_transform, ground_color );		


	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 100, 0, -20 ) );
	model_transform = mult( model_transform, scale( 1, 0.8, 0.8 ) ); 

	this.m_hill.draw( this.graphicsState, model_transform, ground_color );	

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 140, 0, 0 ) );
	model_transform = mult( model_transform, scale( 1.2, 0.8, 0.8 ) ); 

	this.m_hill.draw( this.graphicsState, model_transform, ground_color );	

	return model_transform;
}

Animation.prototype.draw_jedi = function(jedi) {
	var grey = new Material( vec4( 0.27, 0.27, 0.27), 1, 1, 1, 40 );
	var cloak_color = new Material( vec4( 107/255, 65/255, 37/255), 1, 1, 1, 40 );
	var leg_color = new Material( vec4( 161/255, 144/255, 82/255), 1, 1, 1, 40 );
	var skin_color = new Material( vec4( 237/255, 191/255, 127/255), 1, 1, 1, 40 );

	var all_transform = jedi.all;
	all_transform = mult( all_transform, translate( 150, -1, 520) ); 
	all_transform = mult( all_transform, rotate( 180, 0, 1, 0) ); 
	all_transform = mult( all_transform, scale( 1.75, 1.75, 1.75 ) ); 

	var stack = [];
	stack.push(all_transform);

	this.m_jedi_cloak.draw( this.graphicsState, all_transform, cloak_color );
	this.m_jedi_head.draw( this.graphicsState, all_transform, skin_color );
	this.m_jedi_nose.draw( this.graphicsState, all_transform, skin_color );
	this.m_jedi_body.draw( this.graphicsState, all_transform, skin_color );

	all_transform = mult( all_transform, translate( 1, 0, 0) );
	this.m_jedi_leg.draw( this.graphicsState, all_transform, grey );

	all_transform = stack.pop(); 
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( -1, 0, 0) );
	this.m_jedi_leg.draw( this.graphicsState, all_transform, grey );

	all_transform = stack.pop(); 
	stack.push(all_transform);

	this.m_jedi_lightsaber.draw( this.graphicsState, all_transform, grey );
}

Animation.prototype.draw_droid = function(droid, offset) {
	var brown = new Material( vec4( 114/255, 83/255, 47/255 ), 1, 1, 1, 40 );
	var darkbrown = new Material( vec4( 43/255, 32/255, 18/255), 1, 1, 1, 40 );

	var all_transform = droid.all;
	all_transform = mult( all_transform, rotate( 90, 0, 1, 0) ); 

	var stack = [];
	stack.push(all_transform);



	all_transform = mult( all_transform, translate( 20, 10, 140 + offset) ); 
	// all_transform = mult( all_transform, scale( 2, 2, 2 ) ); 

	stack.push(all_transform);
	all_transform = mult( all_transform, scale( 0.5, 1, 1 ) ); 
	this.m_droid_head1.draw( this.graphicsState, all_transform, brown );	
	this.m_droid_head2.draw( this.graphicsState, all_transform, brown );
	this.m_droid_head3.draw( this.graphicsState, all_transform, brown );
	this.m_droid_head4.draw( this.graphicsState, all_transform, darkbrown );

	all_transform = stack.pop();
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( 0, -3, 0 ) ); 
	all_transform = mult( all_transform, scale( 1.2, 1.2, 1.6 ) ); 
	this.m_droid_body.draw( this.graphicsState, all_transform, brown );	

	all_transform = stack.pop();
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( 0, -6, 0 ) );
	this.draw_droid_legs(all_transform);

	all_transform = stack.pop();
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( -1.5, -3, 0 ) );
	all_transform = mult( all_transform, rotate( 20 * Math.sin(this.graphicsState.animation_time/200), 1, 1, 0 ) );
	this.m_droid_blaster.draw( this.graphicsState, all_transform, darkbrown );

	droid.offsetX = all_transform[0][3];

}	

Animation.prototype.draw_droid_legs = function(model_transform) {
	this.draw_droid_leg(model_transform, 1);
	this.draw_droid_leg(model_transform, -1);
	return model_transform;
}

Animation.prototype.draw_droid_leg = function(model_transform, orientation) {
	var brown = new Material( vec4( 114/255, 83/255, 47/255 ), 1, 1, 1, 40 );
	var darkbrown = new Material( vec4( 43/255, 32/255, 18/255), 1, 1, 1, 40 );

	model_transform = mult( model_transform, translate( 0, 0, 0.75 * orientation ) );  

	var stack = [];
	stack.push(model_transform);

	model_transform = mult( model_transform, rotate( 15 * orientation * Math.sin(this.graphicsState.animation_time/200), 0, 0, 1 ) );

	stack.push(model_transform);

	model_transform = mult( model_transform, scale( 1, 1.4, 1 ) ); 
	this.m_droid_leg1.draw( this.graphicsState, model_transform, brown );

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 0, -1, 0 ) ); 
	model_transform = mult( model_transform, scale( .3, .3, .3 ) ); 
	this.m_sphere.draw( this.graphicsState, model_transform, darkbrown );

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 0, -1, 0 ) ); 
		model_transform = mult( model_transform, rotate( 25 * orientation * Math.sin(this.graphicsState.animation_time/200), 0, 0, 1 ) );
	model_transform = mult( model_transform, translate( 0, -2, 0 ) ); 
	model_transform = mult( model_transform, scale( 1, 1.4, 1 ) ); 
	

	this.m_droid_leg3.draw( this.graphicsState, model_transform, brown );

	return model_transform;
}

Animation.prototype.draw_BB = function(bb) {
	var t = new Material( vec4( .5,.5,.5,1 ), 1, 1, 1, 40, "bb8.png" );
	var grey = new Material( vec4( 0.27, 0.27, 0.27), 1, 1, 1, 40 );
	var black = new Material( vec4( 0, 0, 0), 1, 1, 1, 40 );

	var all_transform = mult( bb.all, translate( 150, 6, -20 ) ); 
	var body_transform = bb.body;
	var head_transform = bb.head;

	all_transform = mult( all_transform, scale( .67, .67, .67 ) );

	var stack = [];
	stack.push(all_transform);

	all_transform = mult( all_transform, rotate( -90, 0, 0, 1 ) );

	// model_transform = mult( model_transform, translate( 0, 2, 0.5 * -orientation) );
		// head_transform = mult( head_transform, rotate( 17.5 * Math.sin(this.graphicsState.animation_time/200), 0, 1, 0 ) );
	// model_transform = mult( model_transform, translate( 0, -2, 0.5 * orientation ) );
	all_transform = mult( all_transform, scale( 3, 3, 3 ) ); // small obj file	

	this.m_top_half.draw( this.graphicsState, mult(all_transform, head_transform), t );	
	this.m_bottom_half.draw( this.graphicsState, mult(all_transform, head_transform), grey );	

	all_transform = mult( all_transform, translate( 1.6, 0, 0 ) );
	all_transform = mult( all_transform, scale( 1.5, 1.5, 1.5 ) );	
	all_transform = mult( all_transform, rotate( 90, 1, 0, 0 ) );
	// all_transform = mult( all_transform, rotate( -this.graphicsState.animation_time/2, 0, 0, 1 ) );
	this.m_sphere.draw( this.graphicsState, mult(all_transform, body_transform), t );	

	all_transform = stack.pop();
	all_transform = mult( all_transform, rotate( -90, 0, 0, 1 ) );
	all_transform = mult(all_transform, head_transform);
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( -4, -1, 0 ) );
	all_transform = mult( all_transform, scale( 3, 0.1, 0.1 ) );	
	all_transform = mult( all_transform, rotate( 90, 0, 1, 0 ) );
	this.m_cylinder.draw( this.graphicsState, all_transform, grey );

	all_transform = stack.pop();
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( -3, 1, 0 ) );
	all_transform = mult( all_transform, scale( 1.2, 0.1, 0.1 ) );	
	all_transform = mult( all_transform, rotate( 90, 0, 1, 0 ) );
	this.m_cylinder.draw( this.graphicsState, all_transform, grey );

	all_transform = stack.pop();
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( -0.75, 1.25, 2.25 ) );
	all_transform = mult( all_transform, scale( 0.45, 0.45, 0.45 ) );	
	this.m_sphere.draw( this.graphicsState, all_transform, grey );	

	all_transform = stack.pop();
	stack.push(all_transform);

	all_transform = mult( all_transform, translate( -1.725, 0, 2.5 ) );
	all_transform = mult( all_transform, scale( 0.75, 0.75, 0.75 ) );	
	this.m_sphere.draw( this.graphicsState, all_transform, grey );	


	return bb;
}

Animation.prototype.update_strings = function( debug_screen_object )		// Strings this particular class contributes to the UI
{
	debug_screen_object.string_map["time"] = "Animation Time: " + this.graphicsState.animation_time/1000 + "s";
	debug_screen_object.string_map["fps"] = "FPS: " + Math.round(1000 / this.animation_delta_time);

	debug_screen_object.string_map["basis"] = "Showing basis: " + this.m_axis.basis_selection;
	debug_screen_object.string_map["animate"] = "Animation " + (animate ? "on" : "off") ;
	debug_screen_object.string_map["thrust"] = "Thrust: " + thrust;
}
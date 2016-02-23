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
		
		gl.clearColor( 0, 0, 0, 1 );			// Background color

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

		// JEDI OBJECTS
		self.m_jedi_cloak = new shape_from_file( "jedi-cloak.obj" );
		self.m_jedi_head = new shape_from_file( "jedi-head.obj" );
		self.m_jedi_body = new shape_from_file( "jedi-body.obj" );
		self.m_jedi_leg = new shape_from_file( "jedi-leg.obj" );
		self.m_jedi_lightsaber = new shape_from_file( "jedi-lightsaber.obj" );

		self.m_plane = new plane( mat4() );
		
		// 1st parameter is camera matrix.  2nd parameter is the projection:  The matrix that determines how depth is treated.  It projects 3D points onto a plane.
		self.graphicsState = new GraphicsState( translate(0, 0,-40), perspective(45, canvas.width/canvas.height, .1, 1000), 0 );

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
	
	var purplePlastic = new Material( vec4( .9,.5,.9,1 ), 1, 1, 1, 40 ), // Omit the string parameter if you want no texture
		greyPlastic = new Material( vec4( .5,.5,.5,1 ), 1, 1, .5, 20 ),
		earth = new Material( vec4( .5,.5,.5,1 ), 1, 1, 1, 40, "earth.gif" );
		
	/**********************************
	Start coding here!!!!
	**********************************/

	var stack = []; 
	stack.push(model_transform);	
											
													
	// this.draw_ground(model_transform); 
	// this.m_plane.draw( this.graphicsState, model_transform, greyPlastic );	
	// model_transform = mult( model_transform, translate( 0, 10, 0 ) );	

	this.draw_jedi(model_transform);

	model_transform = mult( model_transform, translate( 10, 0, 0 ) );	
	this.draw_BB(model_transform);

	// model_transform = mult( model_transform, translate( 0, 10, 0 ) );	
	// this.draw_droid(model_transform);
	
}

Animation.prototype.draw_jedi = function(model_transform) {
	var grey = new Material( vec4( 0.27, 0.27, 0.27), 1, 1, 1, 40 );

	var stack = [];
	stack.push(model_transform);

	this.m_jedi_cloak.draw( this.graphicsState, model_transform, grey );
}

Animation.prototype.draw_droid = function(model_transform) {
	var grey = new Material( vec4( 0.27, 0.27, 0.27), 1, 1, 1, 40 );

	var stack = [];
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 0, 8, 0 ) ); 
	model_transform = mult( model_transform, scale( 3, 3, 3 ) ); 

	stack.push(model_transform);
	model_transform = mult( model_transform, scale( 0.5, 1, 1 ) ); 
	this.m_droid_head1.draw( this.graphicsState, model_transform, grey );	
	this.m_droid_head2.draw( this.graphicsState, model_transform, grey );
	this.m_droid_head3.draw( this.graphicsState, model_transform, grey );
	this.m_droid_head4.draw( this.graphicsState, model_transform, grey );

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 0, -3, 0 ) ); 
	model_transform = mult( model_transform, scale( 1.2, 1.2, 1.6 ) ); 
	this.m_droid_body.draw( this.graphicsState, model_transform, grey );	

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 0, -6, 0 ) );
	this.draw_droid_legs(model_transform);

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( -1.5, -3, 0 ) );
	this.m_droid_blaster.draw( this.graphicsState, model_transform, grey );

}	

Animation.prototype.draw_droid_legs = function(model_transform) {
	this.draw_droid_leg(model_transform, 1);
	this.draw_droid_leg(model_transform, -1);
	return model_transform;
}

Animation.prototype.draw_droid_leg = function(model_transform, orientation) {
	var grey = new Material( vec4( 0.27, 0.27, 0.27 ), 1, 1, 1, 40 );

	model_transform = mult( model_transform, translate( 0, 0, 0.75 * orientation ) );  

	var stack = [];
	stack.push(model_transform);

	model_transform = mult( model_transform, scale( 1, 1.4, 1 ) ); 
	this.m_droid_leg1.draw( this.graphicsState, model_transform, grey );

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 0, -1, 0 ) ); 
	model_transform = mult( model_transform, scale( .3, .3, .3 ) ); 
	this.m_sphere.draw( this.graphicsState, model_transform, grey );

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 0, -3, 0 ) ); 
	model_transform = mult( model_transform, scale( 1, 1.4, 1 ) ); 
	this.m_droid_leg3.draw( this.graphicsState, model_transform, grey );

	return model_transform;
}

Animation.prototype.draw_BB = function(model_transform) {
	var t = new Material( vec4( .5,.5,.5,1 ), 1, 1, 1, 40, "bb8.png" );
	var stars = new Material( vec4( .5,.5,.5,1 ), 1, 1, 1, 40, "stars.png" );

	var grey = new Material( vec4( 0.27, 0.27, 0.27), 1, 1, 1, 40 );

	var stack = [];
	stack.push(model_transform);

	model_transform = mult( model_transform, rotate( -90, 0, 0, 1 ) );
	model_transform = mult( model_transform, scale( 3, 3, 3 ) ); // small obj file	

	this.m_top_half.draw( this.graphicsState, model_transform, stars );	
	this.m_bottom_half.draw( this.graphicsState, model_transform, stars );	

	model_transform = mult( model_transform, translate( 1.6, 0, 0 ) );
	model_transform = mult( model_transform, scale( 1.5, 1.5, 1.5 ) );	
	this.m_sphere.draw( this.graphicsState, model_transform, t );	

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( 1, 4, 0 ) );
	model_transform = mult( model_transform, scale( 0.1, 2.5, 0.1 ) );	
	model_transform = mult( model_transform, rotate( 90, 1, 0, 0 ) );
	this.m_cylinder.draw( this.graphicsState, model_transform, grey );

	model_transform = stack.pop();
	stack.push(model_transform);

	model_transform = mult( model_transform, translate( -1, 3, 0 ) );
	model_transform = mult( model_transform, scale( 0.1, 1.2, 0.1 ) );	
	model_transform = mult( model_transform, rotate( 90, 1, 0, 0 ) );
	this.m_cylinder.draw( this.graphicsState, model_transform, grey );


	return model_transform;
}

Animation.prototype.draw_ground = function(model_transform) {
	var stack = [];
	stack.push(model_transform);

	var ground_color = new Material( vec4( 0.2, 0.6, 0.0), 1, 1, 1, 40 );

	model_transform = mult( model_transform, scale( 100, 1, 100 ) );												
	this.m_cube.draw( this.graphicsState, model_transform, ground_color );		

	model_transform = stack.pop();
	return model_transform;
}

Animation.prototype.update_strings = function( debug_screen_object )		// Strings this particular class contributes to the UI
{
	debug_screen_object.string_map["time"] = "Animation Time: " + this.graphicsState.animation_time/1000 + "s";
	debug_screen_object.string_map["basis"] = "Showing basis: " + this.m_axis.basis_selection;
	debug_screen_object.string_map["animate"] = "Animation " + (animate ? "on" : "off") ;
	debug_screen_object.string_map["thrust"] = "Thrust: " + thrust;
}
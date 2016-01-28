// *******************************************************
// CS 174a Graphics Example Code
// Shape.js - Defines a number of objects that inherit from the Shape class.  Each manages lists of its own vertex positions, vertex normals, and texture coordinates per vertex.  
// Instantiating a shape automatically calls OpenGL functions to pass each list into a buffer in the graphics card's memory.

// Some utility functions first:

var textures = {};
function initTexture(filename, bool_mipMap) 
	{
		textures[filename] = {} ;
		textures[filename].id 				= gl.createTexture();
		textures[filename].image 			= new Image();    
		textures[filename].image.onload 	= (	function (texture, bool_mipMap) {
			return function( ) {			
				gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
				gl.bindTexture(gl.TEXTURE_2D, texture.id);
				gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, texture.image);
				gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
				if(bool_mipMap)
					{	gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);	gl.generateMipmap(gl.TEXTURE_2D);	}
				else
						gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
				texture.loaded = true;
			}
		}	) (textures[filename], bool_mipMap);
		textures[filename].image.src 		= filename;
	}

function inherit(subType, superType)
	{
		var p = Object.create(superType.prototype);
		p.constructor = subType;
		subType.prototype = p;
	}
function mult_vec(M, v)
{
	v_4 = v.length == 4 ? v : vec4( v, 0 );
	v_new = vec4();
	v_new[0] = dot( M[0], v_4 );
	v_new[1] = dot( M[1], v_4 );
	v_new[2] = dot( M[2], v_4 );
	v_new[3] = dot( M[3], v_4 );
	return v_new;
}

function toMat3( mat4_affine )
	{
		var m = [];
		m.push( mat4_affine[0].slice( 0, 3 ) );
		m.push( mat4_affine[1].slice( 0, 3 ) );
		m.push( mat4_affine[2].slice( 0, 3 ) );
		m.matrix = true;
		return m;
	}
	
	
function Material( color, ambient, diffusivity, shininess, smoothness, texture_filename )
	{
		this.color = color;	this.ambient = ambient;  this.diffusivity = diffusivity; this.shininess = shininess; 
		this.smoothness = smoothness; this.texture_filename = texture_filename;
	}
	
// *******************************************************
// IMPORTANT: When you extend the Shape class, these are the four arrays you must put values into.  Each shape has a list of vertex positions (here just called vertices), vertex normals 
// (vectors that point away from the surface for each vertex), texture coordinates (pixel coordintates in the texture picture, scaled down to the range [ 0.0, 1.0 ] to place each vertex 
// somewhere relative to the picture), and most importantly - indices, a list of index triples defining which three vertices belong to each triangle.  Your class must build these lists 
// and then send them to the graphics card by calling init_buffers().  At some point a simple example will be given of manually building these lists for a shape.
function shape()
	{
		this.vertices = [];
		this.normals = [];
		this.texture_coords = [];
		this.indices = [];
		this.indexed = true;
	}
		
	shape.prototype.flat_normals_from_triples = function( offset )		// This calculates normals automatically for flat shaded elements, assuming that each element is independent (no shared vertices)
		{
			this.normals.length = this.vertices.length;
			for( var counter = offset; counter < ( this.indexed ? this.indices.length : this.vertices.length ) ; counter += 3 )
			{
				var a = this.vertices[ this.indexed ? this.indices[ counter     ] : counter ] ;
				var b = this.vertices[ this.indexed ? this.indices[ counter + 1 ] : counter + 1 ] ;
				var c = this.vertices[ this.indexed ? this.indices[ counter + 2 ] : counter + 2 ] ;
						
				var triangleNormal = normalize( cross( subtract(a, b), subtract(c, a)) );		// Cross two edge vectors of this triangle together to get the normal
				if( length( add( triangleNormal, a) ) < length(a) )
						scale_vec( -1, triangleNormal );										// Flip the normal if for some point it brings us closer to the origin
				
				this.normals[ this.indices[ counter     ] ] = vec3( triangleNormal[0], triangleNormal[1], triangleNormal[2] );
				this.normals[ this.indices[ counter + 1 ] ] = vec3( triangleNormal[0], triangleNormal[1], triangleNormal[2] );
				this.normals[ this.indices[ counter + 2 ] ] = vec3( triangleNormal[0], triangleNormal[1], triangleNormal[2] );
			}
		};
	
	shape.prototype.spherical_texture_coords = function( vert_index )
		{	this.texture_coords.push( vec2( .5 + Math.atan2( this.vertices[vert_index][2], this.vertices[vert_index][0] ) / 2 / Math.PI, .5 - 2 * Math.asin( this.vertices[vert_index][1] ) / 2 / Math.PI ) );
		}
	
	shape.prototype.init_buffers = function()			// Send the completed vertex and index lists to their own buffers in the graphics card.
		{
			this.graphics_card_buffers = [];	// Memory addresses of the buffers given to this shape in the graphics card.
			for( var i = 0; i < 4; i++ )
			{
				this.graphics_card_buffers.push( gl.createBuffer() );
				gl.bindBuffer(gl.ARRAY_BUFFER, this.graphics_card_buffers[i] );
				switch(i) {
					case 0: gl.bufferData(gl.ARRAY_BUFFER, flatten(this.vertices), gl.STATIC_DRAW); break;
					case 1: gl.bufferData(gl.ARRAY_BUFFER, flatten(this.normals), gl.STATIC_DRAW); break;
					case 2: gl.bufferData(gl.ARRAY_BUFFER, flatten(this.texture_coords), gl.STATIC_DRAW); break;	}
			}
			
			if( this.indexed )
			{
				this.index_buffer = gl.createBuffer();
				gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.index_buffer);
				gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(this.indices), gl.STATIC_DRAW);
			}
		};
	
	
	shape.prototype.update_uniforms = function( graphicsState, model_transform, material )			// Send the current matrices to the shader
		{
				var camera_model_transform 				= mult( graphicsState.camera_transform, model_transform );
				var projection_camera_model_transform 	= mult( graphicsState.projection_transform, camera_model_transform );
				var camera_model_transform_normal		= toMat3( transpose( inverse( camera_model_transform ) ) );
				
				gl.uniformMatrix4fv( g_addrs.camera_transform_loc, 					false, flatten( graphicsState.camera_transform ) );
				gl.uniformMatrix4fv( g_addrs.camera_model_transform_loc, 			false, flatten( camera_model_transform ) );
				gl.uniformMatrix4fv( g_addrs.projection_camera_model_transform_loc, false, flatten( projection_camera_model_transform ) );
				gl.uniformMatrix3fv( g_addrs.camera_model_transform_normal_loc, 	false, flatten( camera_model_transform_normal ) );
				   
				var N_LIGHTS = 2, lightPositions = [], lightColors = [], lightPositions_flattened = [], lightColors_flattened = [];
				lightPositions.push( vec4( 100, 0, 0, 1 ) );			lightColors.push( vec4( 0, 0, 1, 1 ) );
				lightPositions.push( vec4( 0, 100, 0, 1 ) );			lightColors.push( vec4( 1, 0, 0, 1 ) );
				for( var i = 0; i < 4 * N_LIGHTS; i++ )
				{
					lightPositions_flattened[i] = lightPositions[ Math.floor(i/4) ][i%4];
					lightColors_flattened[i]    =    lightColors[ Math.floor(i/4) ][i%4];
				}

				gl.uniform4fv( g_addrs.lightPosition_loc, 	lightPositions_flattened );
				gl.uniform4fv( g_addrs.lightColor_loc, 	lightColors_flattened );   
					   
				   
				gl.uniform4fv( g_addrs.color_loc, 			material.color );		// Send a desired shape-wide color to the graphics card
				gl.uniform4fv( g_addrs.lightPosition_loc, 	vec4( 10,7,3,0 ) );
				gl.uniform4fv( g_addrs.lightColor_loc, 		vec4( 1,1,1,1 ) );
				gl.uniform1f ( g_addrs.ambient_loc, material.ambient );
				gl.uniform1f ( g_addrs.diffusivity_loc,  material.diffusivity );
				gl.uniform1f ( g_addrs.shininess_loc, material.shininess );
				gl.uniform1f ( g_addrs.smoothness_loc, material.smoothness );
				gl.uniform1f ( g_addrs.animation_time_loc, graphicsState.animation_time / 1000 );
		};
		
		// The same draw call is used for every shape - the calls draw different things due to the different vertex lists we stored in the graphics card for them.
	shape.prototype.draw = function( graphicsState, model_transform, material )
		{
			this.update_uniforms( graphicsState, model_transform, material );
			
			if( material.texture_filename && textures[ material.texture_filename ].loaded )			// Use a non-existent texture string parameter to signal that we don't want to texture this shape.
			{
				g_addrs.shader_attributes[2].enabled = true;
				gl.uniform1f ( g_addrs.USE_TEXTURE_loc, 1 );
				gl.bindTexture(gl.TEXTURE_2D, textures[ material.texture_filename ].id);
			}
			else
				{	gl.uniform1f ( g_addrs.USE_TEXTURE_loc, 0 );		g_addrs.shader_attributes[2].enabled = false;	}
			
			for( var i = 0, it = g_addrs.shader_attributes[0]; i < g_addrs.shader_attributes.length, it = g_addrs.shader_attributes[i]; i++ )
				if( it.enabled )
				{
					gl.enableVertexAttribArray( it.index );
					gl.bindBuffer( gl.ARRAY_BUFFER, this.graphics_card_buffers[i] );
					gl.vertexAttribPointer( it.index, it.size, it.type, it.normalized, it.stride, it.pointer );
				}
				else
					gl.disableVertexAttribArray( it.index );
			
			if( this.indexed )			
			{
				gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, this.index_buffer );
				gl.drawElements( gl.TRIANGLES, this.indices.length, gl.UNSIGNED_SHORT, 0 );
			}
			else
				gl.drawArrays  ( gl.TRIANGLES, 0, this.vertices.length );
		};
		
	
function sphere( points_transform, max_subdivisions )		// Build a sphere using subdivision, starting with a tetrahedron.  Store each level of detail in separate index lists.
	{	
		shape.call(this);	
			
		this.subdivideTriangle = function( a, b, c, recipient, count ) 
		{	
			if( count <= 0)
			{		
				recipient.indices.push(a,b,c);		// Build index list with the nice property that skipping every fourth vertex index takes you down one level of detail, each time				
				return;
			}
			else if( recipient.indices_LOD && recipient.indices_LOD[count] )
				recipient.indices_LOD[count].push(a,b,c);
			
			var ab_vert = normalize( mix( recipient.vertices[a], recipient.vertices[b], 0.5) );
			var ac_vert = normalize( mix( recipient.vertices[a], recipient.vertices[c], 0.5) );
			var bc_vert = normalize( mix( recipient.vertices[b], recipient.vertices[c], 0.5) );	
						
			var ab = recipient.vertices.length;		recipient.vertices.push( ab_vert );	
			var ac = recipient.vertices.length;		recipient.vertices.push( ac_vert );	
			var bc = recipient.vertices.length;		recipient.vertices.push( bc_vert );	

			this.subdivideTriangle( a, ab, ac,  recipient, count - 1 );
			this.subdivideTriangle( ab, b, bc,  recipient, count - 1 );
			this.subdivideTriangle( ac, bc, c,  recipient, count - 1 );
			this.subdivideTriangle( ab, bc, ac, recipient, count - 1 );
		}
		
		if( !arguments.length) return;	// Pass no arguments if you just want to statically call functions up the inheritance chain, and / or populate()
		
		this.indices_LOD = [];
		this.index_buffer_LOD = [];
		for( var i = 1; i <= max_subdivisions; i++ )
			this.indices_LOD[i] = [];
		
		this.populate( this, points_transform, max_subdivisions );
		
		for( var i = 1; i <= max_subdivisions; i++ )
		{
			this.index_buffer_LOD[i] = gl.createBuffer();
			gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.index_buffer_LOD[ i ] );
			gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array( this.indices_LOD[ i ] ), gl.STATIC_DRAW);
		}
		
		this.draw = function( graphicsState, model_transform, material, LOD ) 	
		{ 	
			this.update_uniforms( graphicsState, model_transform, material );

			if( material.texture_filename && textures[ material.texture_filename ].loaded )			// Use a non-existent texture string parameter to signal that we don't want to texture this shape.
			{
				g_addrs.shader_attributes[2].enabled = true;
				gl.uniform1f ( g_addrs.USE_TEXTURE_loc, 1 );
				gl.bindTexture(gl.TEXTURE_2D, textures[ material.texture_filename ].id);
			}
			else
				{	gl.uniform1f ( g_addrs.USE_TEXTURE_loc, 0 );		g_addrs.shader_attributes[2].enabled = false;	}
			
			for( var i = 0, it = g_addrs.shader_attributes[0]; i < g_addrs.shader_attributes.length, it = g_addrs.shader_attributes[i]; i++ )
				if( it.enabled )
				{
					gl.enableVertexAttribArray( it.index );
					gl.bindBuffer( gl.ARRAY_BUFFER, this.graphics_card_buffers[i] );
					gl.vertexAttribPointer( it.index, it.size, it.type, it.normalized, it.stride, it.pointer );
				}
				else
					gl.disableVertexAttribArray( it.index );
				
			if( LOD === undefined || LOD < 0 || LOD + 1 >= this.indices_LOD.length )
			{
				gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, this.index_buffer );
				gl.drawElements( gl.TRIANGLES, this.indices.length, gl.UNSIGNED_SHORT, 0 );
			}
			else
			{
				gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, this.index_buffer_LOD[ this.indices_LOD.length - 1 - LOD ] );
				gl.drawElements( gl.TRIANGLES, this.indices_LOD[ this.indices_LOD.length - 1 - LOD ].length, gl.UNSIGNED_SHORT, 0 );
			}			
		}
		this.init_buffers();
	}
inherit(sphere, shape);

	sphere.prototype.populate = function ( recipient, points_transform, max_subdivisions ) 
		{	
			var offset = recipient.vertices.length;
			recipient.vertices.push(		vec3(0.0, 0.0, -1.0) 				 );
			recipient.vertices.push(		vec3(0.0, 0.942809, 0.333333) 		 );
			recipient.vertices.push(		vec3(-0.816497, -0.471405, 0.333333) );
			recipient.vertices.push(		vec3(0.816497, -0.471405, 0.333333)  );
			
			this.subdivideTriangle( 0 + offset, 1 + offset, 2 + offset, recipient, max_subdivisions);
			this.subdivideTriangle( 3 + offset, 2 + offset, 1 + offset, recipient, max_subdivisions);
			this.subdivideTriangle( 1 + offset, 0 + offset, 3 + offset, recipient, max_subdivisions);
			this.subdivideTriangle( 0 + offset, 2 + offset, 3 + offset, recipient, max_subdivisions); 
			
			for( var i = offset; i < recipient.vertices.length; i++ )
			{
				recipient.spherical_texture_coords( i );
				recipient.normals[i] = recipient.vertices[i].slice();
				recipient.vertices[i] = vec3( mult_vec( points_transform, vec4( recipient.vertices[i], 1 ) ) );	
			}
		};



function triangle_strip()						// Arrange triangles in a strip, where the list of vertices alternates sides.
	{	shape.call(this);	};
inherit(triangle_strip, shape);

	triangle_strip.prototype.init_from_strip_lists = function( recipient, vertices, indices )
		{			
			var offset = recipient.vertices.length;
			[].push.apply( recipient.vertices, vertices );
			
			for( var counter = 0; counter < indices.length - 2; counter++ )
			{
				recipient.indices.push( indices[counter + 2 * ((counter+1) % 2 ) ] + offset );		// The modulus, used as a conditional here, makes face orientations uniform.
				recipient.indices.push( indices[counter + 1] + offset );
				recipient.indices.push( indices[counter + 2 * ( counter    % 2 ) ] + offset );
			}
		};
		
function rectangular_strip( numRectangles, points_transform )
	{
		triangle_strip.call(this);	
		if( !arguments.length) return;	// Pass no arguments if you just want to statically call functions up the inheritance chain, and / or populate()
		this.populate( this, numRectangles, points_transform );
		this.init_buffers();
	}
inherit(rectangular_strip, triangle_strip);

	rectangular_strip.prototype.populate = function( recipient, numRectangles, points_transform )	
				{	
					var offset = recipient.vertices.length;		var index_offset = recipient.indices.length;
					var vertices = [];
					var strip_indices = [];
					var topIdx = 0; var bottomIdx = numRectangles + 1;						
					
					for( var i = 0; i <= numRectangles; i++ )
					{
						vertices[topIdx] 	= vec3( 0,  .5, topIdx - .5 * numRectangles );		recipient.texture_coords[ topIdx + offset ]    = vec2( topIdx / numRectangles, 1 );
						vertices[bottomIdx] = vec3( 0, -.5, topIdx - .5 * numRectangles );		recipient.texture_coords[ bottomIdx + offset ] = vec2( topIdx / numRectangles, 0 );
						strip_indices.push(topIdx++);
						strip_indices.push(bottomIdx++);
					}
					
					this.init_from_strip_lists(recipient, vertices, strip_indices);
					
					for( var i = offset; i < recipient.vertices.length; i++ )
						recipient.vertices[i] = vec3( mult_vec( points_transform, vec4( recipient.vertices[i], 1 ) ) );						
					recipient.flat_normals_from_triples( index_offset );
							
				}
				
function cylindrical_strip( numRectangles, points_transform )
	{
		triangle_strip.call(this);	
		if( !arguments.length) return;	// Pass no arguments if you just want to statically call functions up the inheritance chain, and / or populate()	
		this.populate( this, numRectangles, points_transform );
		this.init_buffers();
	}
inherit(cylindrical_strip, triangle_strip);

	cylindrical_strip.prototype.populate = function( recipient, numRectangles, points_transform )	
				{	
					var vertices = [];
					var strip_indices = [];
					var offset = recipient.vertices.length;		var index_offset = recipient.indices.length;
					var topIdx = 0; var bottomIdx = numRectangles;
						
					for( var i = 0; i < numRectangles; i++ )
					{
						vertices[topIdx] 	= vec3( Math.cos(2 * Math.PI * topIdx / numRectangles), Math.sin(2 * Math.PI * topIdx / numRectangles), .5 );	
						recipient.texture_coords[topIdx + offset]    = vec2(0, topIdx / numRectangles );
						vertices[bottomIdx] = vec3( Math.cos(2 * Math.PI * topIdx / numRectangles), Math.sin(2 * Math.PI * topIdx / numRectangles), -.5 );			
						recipient.texture_coords[bottomIdx + offset] = vec2(1, topIdx / numRectangles );
						strip_indices.push(topIdx++);
						strip_indices.push(bottomIdx++);
					}
					strip_indices.push(0);
					strip_indices.push( numRectangles );
									
					this.init_from_strip_lists(recipient, vertices, strip_indices);
							
					for( var i = offset; i < recipient.vertices.length; i++ )
						recipient.vertices[i] = vec3( mult_vec( points_transform, vec4( recipient.vertices[i], 1 ) ) );					
					recipient.flat_normals_from_triples( index_offset );
				}

function shape_from_file(filename)		// Begin downloading the mesh, and once it completes return control to our webGLStart function
	{
		shape.call(this);
			
		this.draw = function( graphicsState, model_transform, material ) 	{
		 	if( this.ready ) shape.prototype.draw.call(this, graphicsState, model_transform, material );		}	
		
		this.filename = filename;

		this.webGLStart = function(meshes)
			{
				for( var j = 0; j < meshes.mesh.vertices.length/3; j++ )
				{
					this.vertices.push( vec3( meshes.mesh.vertices[ 3*j ], meshes.mesh.vertices[ 3*j + 1 ], meshes.mesh.vertices[ 3*j + 2 ] ) );
					this.texture_coords.push( vec2(meshes.mesh.textures[ 2*j ],meshes.mesh.textures[ 2*j + 1 ]));
				}
				this.indices  = meshes.mesh.indices;	  
				this.normals  = meshes.mesh.vertexNormals;
				this.init_buffers();
				this.ready = true;
			}
		OBJ.downloadMeshes( { 'mesh' : filename }, (function(self) { return self.webGLStart.bind(self) }(this) ) );
	}
inherit(shape_from_file, shape);


function triangle_fan_full( num_tris, points_transform )		// Arrange triangles in a fan.  This version goes all the way around a circle with them.
	{	
		shape.call(this);	
		
		this.createCircleVertices = function( recipient, num_tris ) 
			{	
				for( var counter = 0; counter++ <= num_tris;   )
				{
						recipient.vertices.push( vec3( Math.cos(2 * Math.PI * counter/num_tris), Math.sin(2 * Math.PI * counter/num_tris), -1 ) );		
						recipient.texture_coords.push( vec2( counter/num_tris, 1 ) );	
				}
			}
		
		this.initFromSequence = function( recipient, center_idx, num_tris, offset )
			{	
				for(var index = offset; index <= offset + num_tris;	 )
				{
					recipient.indices.push( index );
					recipient.indices.push( center_idx );
					recipient.indices.push( ++index );
				}
				recipient.indices.pop(); 
				recipient.indices.push( offset );
			}
		
		if( !arguments.length) return;	// Pass no arguments if you just want to statically call functions up the inheritance chain, and / or populate()
		this.populate( this, num_tris, points_transform );
		this.init_buffers();
	}
inherit(triangle_fan_full, shape);

	triangle_fan_full.prototype.populate = function( recipient, num_tris, points_transform, center_idx )
		{
			if( center_idx === undefined )			// Not re-using a point?  Create one.
			{
				center_idx = recipient.vertices.push( vec3( mult_vec( points_transform, vec4( 0,0,1,1 ) ) ) ) - 1;
				recipient.texture_coords.push( vec2( 1, 0 ) );
			}				
			var offset = recipient.vertices.length;		var index_offset = recipient.indices.length;				
			
			this.createCircleVertices( recipient, num_tris );
			this.initFromSequence(	   recipient, center_idx, num_tris, offset );
			
			recipient.flat_normals_from_triples( index_offset );	
		
			for( var i = offset; i < recipient.vertices.length; i++ )
				recipient.vertices[i] = vec3( mult_vec( points_transform, vec4( recipient.vertices[ i ], 1 ) ) );	
		};

function cube( points_transform )		
	{
		shape.call(this);
		this.populate( this, mat4() );
		this.init_buffers();
	}
inherit(cube, shape);

	cube.prototype.populate = function( recipient, points_transform )
	{
			var m_strip = new rectangular_strip(); 
			for( var i = 0; i < 3; i++ )										// Build a cube by inserting six triangle strips into the lists.
				for( var j = 0; j < 2; j++ )
					m_strip.populate( recipient, 1, mult( points_transform, mult( rotate( 90, vec3( i==0, i==1, i==2 ) ), translate( j - .5, 0, 0 ) ) ) );
	}
	


function axis()
	{
		shape.call(this);
		
		this.basis_selection = 0;
		this.drawOneAxis = function(object_transform)
		{
			var original = object_transform;
			object_transform = mult( object_transform, translate(0, 0, 4));
			object_transform = mult( object_transform, scale(.25, .25, .25));
			this.m_fan.populate ( this, 10, object_transform );
			object_transform = original;
			object_transform = mult( object_transform, translate(1, 1, .5));
			object_transform = mult( object_transform, scale(.1, .1, 1));
			this.m_cube.populate( this, object_transform );
			object_transform = original;
			object_transform = mult( object_transform, translate(1, 0, .5));
			object_transform = mult( object_transform, scale(.1, .1, 1));
			this.m_cube.populate( this, object_transform );
			object_transform = original;
			object_transform = mult( object_transform, translate(0, 1, .5));
			object_transform = mult( object_transform, scale(.1, .1, 1));
			this.m_cube.populate( this, object_transform );
			object_transform = original;			
			object_transform = mult( object_transform, translate(0, 0, 2));
			object_transform = mult( object_transform, scale(.1, .1, 4));
			this.m_cylinder.populate( this, 7, object_transform );
		}
		
		this.populate = ( function (self) 
			{	
				self.m_sphere = new sphere(); self.m_cube = new cube(); self.m_cylinder = new cylindrical_strip(); self.m_fan = new triangle_fan_full;
				var stack = [];				
				var object_transform = mat4();
				object_transform = mult( object_transform, scale(.25, .25, .25));
				self.m_sphere.populate( self, object_transform, 3 );
				object_transform = mat4();
				self.drawOneAxis(object_transform);
				object_transform = mult( object_transform, rotate(-90, vec3(1,0,0)));
				object_transform = mult( object_transform, scale(1, -1, 1));
				self.drawOneAxis(object_transform);
				object_transform = rotate(90, vec3(0,1,0));
				object_transform = mult( object_transform, scale(-1, 1, 1));
				self.drawOneAxis(object_transform);				
			} )(this);
			
																													// Only draw this set of axes if it is the one selected through the user interface.
		this.draw = function( current, graphicsState, model_transform, material ) 	{ 	
			if( this.basis_selection == current ) shape.prototype.draw.call(this, graphicsState, model_transform, material );	}	
			
		this.init_buffers();
	}
inherit(axis, shape);
			
			
			
function text_line( string )		// Draws a rectangle textured with images of ASCII characters over each quad, spelling out a string.
	{
		shape.call(this);
		
		this.populate = ( function ( self, max_size ) 
			{	
				self.max_size = max_size;
				var object_transform = mat4();
				for( var i = 0; i < max_size; i++ )
				{
					rectangular_strip.prototype.populate( self, 1, object_transform );
					object_transform = mult( object_transform, translate( 0, 0, -.7 ));
				}
			} )( this, string );
			
		this.init_buffers();
		
		this.draw = function( graphicsState, model_transform, heads_up_display, color ) 
			{
				if( heads_up_display )			{	gl.disable( gl.DEPTH_TEST );	var temp_camera_transform = graphicsState.camera_transform;	graphicsState.camera_transform = mat4();	}
				shape.prototype.draw.call(this, graphicsState, model_transform, new Material( color, 1, 1, 1, 40, "text.png" ) );	
				if( heads_up_display )			{	gl.enable(  gl.DEPTH_TEST );		graphicsState.camera_transform = temp_camera_transform;	}
			}
			
		this.set_string = function( line )
			{
				for( var i = 0; i < this.max_size; i++ )
					{
						var row = Math.floor( ( i < line.length ? line.charCodeAt( i ) : ' '.charCodeAt() ) / 16 ),
							col = Math.floor( ( i < line.length ? line.charCodeAt( i ) : ' '.charCodeAt() ) % 16 );
							
						var skip = 3, size = 32, sizefloor = size - skip;
						var dim = size * 16, 	left  = (col * size + skip) / dim, 			top    = (row * size + skip) / dim, 
												right = (col * size + sizefloor) / dim, 	bottom = (row * size + sizefloor + 5) / dim;
						
						this.texture_coords[ 4 * i ]	 = vec2( right, 1 - top );
						this.texture_coords[ 4 * i + 1 ] = vec2( left,  1 - top );
						this.texture_coords[ 4 * i + 2 ] = vec2( right, 1 - bottom );
						this.texture_coords[ 4 * i + 3 ] = vec2( left,  1 - bottom );
					}

				gl.bindBuffer( gl.ARRAY_BUFFER, this.graphics_card_buffers[2]);
				gl.bufferData( gl.ARRAY_BUFFER, flatten(this.texture_coords), gl.STATIC_DRAW );
			}

	}
inherit(text_line, shape);

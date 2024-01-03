//
// OtherDrawables.cpp
//	Drawable Object Abstract Factory
//
// See header file comment for overview.
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "OtherDrawables.h"

#include "MeshObject.h"
#include "UniformBufferLiterals.h"
#include "FixedRenderable.h"
#include "VertexNull.h"

extern MeshObject Quad2DTextured;
extern MeshObject Quad2DTextureTinted;
extern MeshObject Quad2DColored;
extern MeshObject Triangle2DColored;

extern UBO_MVP	MVP;
extern UBO uboMVP;

UBO_rtm RayCast;
UBO uboRayCast(RayCast);


PlatformSDL* OtherDrawables::pPlatform;


// Okay, "pseudo-factory" because it doesn't actually instantiate the objects...
//
DrawableProperties& OtherDrawables::GetNextDrawable()
{
	static DrawableProperties demoSpecifiers[] = {
		{
			.name =	"Fragment Shader full-screen color gradient",		// 0
			.mesh = ShaderSets3Vertices,
			.shaders = { { VERTEX,	 "FullScreenTriangle-vert.spv"		},
						 { FRAGMENT, "basicPlaid-frag.spv"				} }
		},{
			.name = "Triangle (vertex buffer, colored per vertex)",		// 1
			.mesh = Triangle2DColored,											// This vertex buffer required by...
			.shaders = { { VERTEX,	 "17_shader_vertexbuffer-vert.spv"	},		//	these shaders.
						 { FRAGMENT, "17_shader_vertexbuffer-frag.spv"	} }
		},{
			.name = "Quad 2D (vertex + index buffer, color vertices)",	// 2
			.mesh = Quad2DColored,
			.shaders = { { VERTEX,	 "17_shader_vertexbuffer-vert.spv"	},
						 { FRAGMENT, "17_shader_vertexbuffer-frag.spv"	} }
		},{
			.name = "Quad 3D (uniform buffer, color vertex)",			// 3
			.mesh = Quad2DColored,												// This quad is required by...
			.shaders = { { VERTEX,	 "21_shader_ubo-vert.spv"			},		//	this shader, which also expects...
						 { FRAGMENT, "21_shader_ubo-frag.spv"			} },
			.pUBOs	 = { uboMVP },												//	this uniform buffer.
			.updateMethod = updateSpinOnZAxis
		},{
			.name = "Textured Quad with colored vertices",				// 4
			.mesh = Quad2DTextureTinted,
			.shaders  = { { VERTEX,   "25_shader_textures-vert.spv"		},
						  { FRAGMENT, "25_shader_textures-frag.spv"		} },	// and now requires...
			.pUBOs	  = { uboMVP },
			.textures = { { "texture.jpg" } },									//	a texture too (array).
			.updateMethod = updateSpinOnZAxis
		},{
			.name = "Minimal Textured Quad",							// 5
			.mesh = Quad2DTextured,												// This quad, required by...
			.shaders  = { { VERTEX,   "TexturedQuad-vert.spv"			},		//	this shader, also requires...
						  { FRAGMENT, "TexturedQuad-frag.spv"			} },
			.pUBOs	  = { uboMVP },												//	this model/view/projection,
			.textures = { { "texture.jpg" } },									//	and this texture.
			.updateMethod = updateSpinOnZAxis
		},{
			.name = "Ray Marching test",								// 6
			.mesh = ShaderSets3Vertices,
			.shaders  = { { VERTEX,   "FullScreenTriangle-vert.spv"		},
						  { FRAGMENT, "tjVolcanic-frag.spv"				} },
			.pUBOs	  = { uboRayCast },
			.textures =	{ { "Noise256.png", LINEAR, REPEAT },
						  { "MossyBark.jpg", MIPMAP, REPEAT },
						  { "PockScorch.jpg", MIPMAP, REPEAT } },
			.updateMethod = updateRayCast
		}
	};
	return (demoSpecifiers[++iCurrentDemo % N_ELEMENTS_IN_ARRAY(demoSpecifiers)]);
}


// "Step forward" all active game elements (e.g. those that move)
//	according to the time that has passed since the previous step.

bool OtherDrawables::updateSpinOnZAxis(GameClock& time)
{
	const float degreesPerSecond = 90.0f;
	float radiansRotation = time.secondsElapsed() * radians(degreesPerSecond);

	const vec3 aroundZaxis = vec3(0.0f, 0.0f, 1.0f);
	const mat4 identityMatrix = mat4(1.0f);

	MVP.model = glm::rotate(identityMatrix, radiansRotation, aroundZaxis);

	return true;	// request caller to: recalculateProjectionIfChanged();
}

bool OtherDrawables::updateRayCast(GameClock& time)
{
	RayCast.resolution	= vec4(float(pPlatform->pixelsWide), float(pPlatform->pixelsHigh), 1.0f, 0.0f);
	RayCast.time		= time.secondsElapsed();
	RayCast.mouse		= vec4(float(pPlatform->mouseX), float(pPlatform->mouseY), 0.0f, 0.0f);

	return false;	// refresh is entirely via fragment shader
}

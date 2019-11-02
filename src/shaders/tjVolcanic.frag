#version 450
#extension GL_ARB_separate_shader_objects : enable

// Created by inigo quilez - iq/2013
// Edit by Tadd
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.


layout(origin_upper_left) in vec4	gl_FragCoord;

layout(location = 0) out vec4		fragColor;

layout(binding = 0)  uniform UniformBufferObject {
	vec4	iResolution;
	float	iTime;
	vec4	iMouse;
} ubo;

layout(binding = 1)  uniform sampler2D	iChannel0;	// noise map
layout(binding = 2)  uniform sampler2D	iChannel1;	// upper terrain texture
layout(binding = 3)  uniform sampler2D	iChannel2;	// lower lava texture


//#define HIGH_QUALITY_NOISE

// Given a 3D point in space, return a repeatable-but-continuous "random noise" value for that point.
// This "3D cloud" of noise is calculated from a 2D noise map
//
float noise(in vec3 x)
{
	vec3 p = floor(x);		// 3D vector of integer parts
	vec3 f = fract(x);			// 3D vector of fractional parts
	f = f * f * (3.0 - 2.0 * f);	// simulate "smoothstep" of fractional parts (e.g. trilinear interpolation function)
#ifndef HIGH_QUALITY_NOISE
	vec2 uv = (p.xy + vec2(37.0, 17.0) * p.z) + f.xy;				// 37 and 17 are primes, providing non-repetitive offset to different noise locale per Z, adding smoothed fractional part
	vec2 rg = textureLod(iChannel0, (uv + 0.5) / 256.0, 0.0).yx;	// from random 256x256 texture, pull from texel center, level-of-detail 0.  div by 256 ranges to 0.0 - 0.9repeating, integer part ignored
																	//	textureLod returns a vector4 or RGBA color... here only R and G values used 
#else
	vec2 uv  = (p.xy + vec2(37.0, 17.0) * p.z);
	vec2 rg1 = textureLod(iChannel0, (uv + vec2(0.5, 0.5)) / 256.0, 0.0).yx;	// texel center
	vec2 rg2 = textureLod(iChannel0, (uv + vec2(1.5, 0.5)) / 256.0, 0.0).yx;	//	texel to right
	vec2 rg3 = textureLod(iChannel0, (uv + vec2(0.5, 1.5)) / 256.0, 0.0).yx;	//	texel below
	vec2 rg4 = textureLod(iChannel0, (uv + vec2(1.5, 1.5)) / 256.0, 0.0).yx;	//	texel diagonally lower right
	vec2 rg  = mix(mix(rg1, rg2, f.x), mix(rg3, rg4, f.x), f.y);
#endif	
	return mix(rg.x, rg.y, f.z);	// linearly interpolate between the two random values per the (already smoothstepped) fractional's Z value
}

float noise(in vec2 x)
{
	vec2 p = floor(x);
	vec2 f = fract(x);
	vec2 uv = p.xy + f.xy * f.xy * (3.0 - 2.0 * f.xy);				// add integer part to similarly "smoothstep"ped fractional
	return textureLod(iChannel0, (uv + 118.4) / 256.0, 0.0).x;		//	again the apparent "local offset" = "118.4" is somewhat of a mystery here... 16 * 37 / 5 ? use of primes?  
}

vec4 texcube(sampler2D tex, in vec3 pos3d, in vec3 norm)
{
	vec4 x = texture(tex, pos3d.yz);
	vec4 y = texture(tex, pos3d.zx);
	vec4 z = texture(tex, pos3d.xy);
	return x * abs(norm.x) + y * abs(norm.y) + z * abs(norm.z);
}

//=====================================================================

float lava(vec2 p)
{
	p += vec2(2.0, 4.0);

	float f;
	f  = 0.5000 * noise(p); p = p * 2.02;
	f += 0.2500 * noise(p); p = p * 2.03;
	f += 0.1250 * noise(p); p = p * 2.01;
	f += 0.0625 * noise(p);

	return f;
}

const mat3 m = mat3( 0.00,  0.80,  0.60,
					-0.80,  0.36, -0.48,
					-0.60, -0.48,  0.64 );

float displacement(vec3 p)
{
	p += vec3(1.0, 0.0, 0.8);

	float f;
	f  = 0.5000 * noise(p); p = m * p * 2.02;	// then move the position in the noise, somewhat
	f += 0.2500 * noise(p); p = m * p * 2.03;	//	and add-in double the amount of noise detail
	f += 0.1250 * noise(p); p = m * p * 2.01;	//	and again...
	f += 0.0625 * noise(p); 

	float n = noise(p * 3.5);
	f += 0.03 * n * n;

	return f;
}

float mapTerrain(in vec3 pos)
{
	return pos.y * 0.1 + (displacement(pos * vec3(0.8, 1.0, 0.8)) - 0.4) * (1.0 - smoothstep(1.0, 3.0, pos.y));
}

float raymarchTerrain(in vec3 ro, in vec3 rd)
{
	float maxd = 30.0;
	float t = 0.1;

	for (int i = 0; i < 256; i++)
	{
		float h = mapTerrain(ro + rd * t);
		if (h < (0.001 * t) || t > maxd)
			break;
		t += h * 0.8;
	}

	if (t > maxd)
		t = -1.0;
	return t;
}

// Calculate a unit normal to the terrain point at "pos" at "t" distance from eye,
//	by getting the terrain points equally to each side ("toSide") of "pos" and
//	taking their difference, for each axis.  "toSide" increases with distance to
//	keep the sampled-point consistent, simulating the divergence of a ray drawn
//	to it, but quickly approximated linearly using a somewhat arbitrary multiplier. 
//
vec3 calcNormal(in vec3 pos, in float t)
{
	const float minimumSamplingDivergence = 0.02;
	const float divergenceFactor = 0.001;

	float toSide = max(minimumSamplingDivergence, divergenceFactor * t);
	vec3 eps = vec3(toSide, 0.0, 0.0);

	return normalize(vec3(mapTerrain(pos + eps.xyy) - mapTerrain(pos - eps.xyy),	// +/- (toSide, 0, 0) on X-axis
						  mapTerrain(pos + eps.yxy) - mapTerrain(pos - eps.yxy),	// +/- (0, toSide, 0) on Y-axis
						  mapTerrain(pos + eps.yyx) - mapTerrain(pos - eps.yyx))	// +/- (0, 0, toSide) on Z-axis
					);
}

vec3 lig = normalize(vec3(-0.3, 0.4, 0.7));

vec4 mapClouds(in vec3 pos)
{
	vec3 q = pos * 0.5 + vec3(0.0, -ubo.iTime, 0.0);

	float d;
	d  = 0.5000 * noise(q); q = q * 2.02;
	d += 0.2500 * noise(q); q = q * 2.03;
	d += 0.1250 * noise(q); q = q * 2.01;
	d += 0.0625 * noise(q);

	d = d - 0.55;
	d *= smoothstep(0.5, 0.55, lava(0.1 * pos.xz) + 0.01);

	d = clamp(d, 0.0, 1.0);

	vec4 res = vec4(d);

	res.xyz  = mix(vec3(1.0, 0.8, 0.7), 0.2 * vec3(0.4, 0.4, 0.4), res.x);
	res.xyz *= 0.25;
	res.xyz *= 0.5 + 0.5 * smoothstep(-2.0, 1.0, pos.y);

	return res;
}

vec4 raymarchClouds(in vec3 ro, in vec3 rd, in vec3 bcol, float tmax)
{
	vec4 sum = vec4(0.0);

	float sun = pow(clamp(dot(rd, lig), 0.0, 1.0), 6.0);
	float t = 0.0;
	for (int i = 0; i < 60; i++)
	{
		if (t > tmax || sum.w > 0.95)
			break;//continue;
		vec3 pos = ro + t * rd;
		vec4 col = mapClouds(pos);

		col.xyz += vec3(1.0, 0.7, 0.4) * 0.4 * sun * (1.0 - col.w);
		col.xyz  = mix(col.xyz, bcol, 1.0 - exp(-0.00006 * t * t * t));

		col.rgb *= col.a;

		sum = sum + col * (1.0 - sum.a);

		t += max(0.1, 0.05 * t);
	}

	sum.xyz /= 0.001 + sum.w;

	return clamp(sum, 0.0, 1.0);
}

float softshadow(in vec3 ro, in vec3 rd, float mint, float k)
{
	float res = 1.0;
	float t = mint;
	for (int i = 0; i < 64; i++)
	{
		float h = mapTerrain(ro + rd * t);
		h = max(h, 0.0);
		res = min(res, k * h / t);
		t += clamp(h, 0.02, 0.5);
		if (res < 0.001)
			break;
	}
	return clamp(res, 0.0, 1.0);
}

#define MOUSE_INPUT

vec3 path(float time)
{
	#ifdef IQ_ORIGINAL
		return vec3(16.0 * cos(0.2 + 0.5 * .1 * time * 1.5), 1.5, 16.0 * sin(0.1 + 0.5 * 0.11 * time * 1.5));
	#elif defined( MOUSE_INPUT )
		return vec3(2.0 * time, 3.0 - 3.0 * ubo.iMouse.y/ubo.iResolution.y, 3.0 - 3.0 * ubo.iMouse.x/ubo.iResolution.x);
	#else // fixed straight-line path
		return vec3(time, 3.0 - 3.0 * 0.5, 3.0 - 3.0 * 0.5);
	#endif
}

// X is forward/backward (forward positive)
// Y is up/down (up positive)
// Z is left/right (right positive)

void moveCamera(float time, out vec3 oRo, out vec3 oTa, out float oCr, out float oFl)
{
	vec3 ro = path(time + 0.0);
	vec3 ta = path(time + 1.6);					// = vec3(ro.x + 5.0, ro.y, ro.z);
	#ifdef IQ_ORIGINAL
		ta.y *= 0.35 + 0.25 * sin(0.09 * time);	// pitch down periodically (ta.y *= 1.0 is straight ahead, ta.y = 0 is down ~ 45 deg)
	#else
		ta.y *= 0.7;
	#endif
	float cr = 0.3 * sin(1.0 + 0.07 * time);	// roll component (pre-transform)  this one ranges from -0.3 to 0.3
	oRo = ro;
	oTa = ta;
	oCr = cr;
	oFl = 2.1;									// "fl" is this constant, perhaps "focal length" or distance from eye to virtual screen
}

mat3 setCamera(in vec3 ro, in vec3 ta, float cr)
{
	vec3 cw = normalize(ta - ro);				// forward vector (say, pointing X positive if moving that way)
	vec3 cp = vec3(sin(cr), cos(cr), 0.0);		// circular unit vector (according to cr) on horizontal plane = vec3(-.3to.3, ~1, 0)
				// -.3:.3      ~1				//		(cr 0: sin 0, cos 1; cr 0.3: sin .3, cos .9; cr -0.3: sin -.3, cos .9)
												//		(e.g. generally points to right, but swings fore/back slightly)
	vec3 cu = normalize(cross(cw, cp));			// "up" perpendicular vector: will always be up if looking directly forward
												//		(will tilt slightly to left/right based on cr)
	vec3 cv = normalize(cross(cu, cw));			// left vector
	return mat3(cu, cv, cw);
}


#define HALF_RENDER	0	// set to 1 to only render every other pixel, for performance comparisons
#define SMOKE		0
#define LAVA		0


//void mainImage(out vec4 fragColor, in vec2 fragCoord)

void main()
{
	vec2 fragCoord = vec2(gl_FragCoord.x, ubo.iResolution.y - gl_FragCoord.y);	// Flip Vulkan's "-Y up" so render isn't upside-down!

	#if HALF_RENDER
		if ((int(fragCoord.x + fragCoord.y) & 1) == 1) //TJ: (render every other pixel, trying to improve frame rate!)
			return;
	#endif

	vec2 q = fragCoord / ubo.iResolution.xy;		// q: current pixel coordinate from 0.0 to 1.0
	vec2 p = -1.0 + 2.0 * q;					// p: current pixel coordinate from -1.0 to 1.0, origin-centered
	p.x *= ubo.iResolution.x / ubo.iResolution.y;		//    with X stretched per screen aspect ratio (say, -1.3 to 1.3 for 4:3 screen)

	// camera
	#ifdef IQ_ORIGINAL		// mouse motion simulates a "rewind like" time-shift
		float off = step(0.001, ubo.iMouse.z) * 6.0 * ubo.iMouse.x / ubo.iResolution.x;
		float time = 3.4 + ubo.iTime + off;
	#else
		const float speed = 1.0f;				// world units per second
		float time = speed * ubo.iTime;
	#endif

	vec3 ro, ta;								// ro: ray origin XYZ		ta: target "look at" XYZ
	float cr, fl;
	moveCamera(time, ro, ta, cr, fl);
	// camera2world transform
	mat3 cam = setCamera(ro, ta, cr);

	// ray	
	vec3 rd = cam * normalize(vec3(p.xy, fl));	// rd: ray direction vector  for current pixel

	// sky	 
	vec3 col = vec3(0.32, 0.36, 0.4) - rd.y * 0.4;		// sky generally "light blue" but slightly darker as angle looking upward increases "darkenUpwards"
	float sun = clamp(dot(rd, lig), 0.0, 1.0);
	col += vec3(1.0, 0.8, 0.4) * 0.2 * pow(sun, 6.0);	// brighten sky color by sunlight color, if looking more towards sun
	col *= 0.9;

	vec3 bcol = col;

	// terrain	
	float t = raymarchTerrain(ro, rd);		// t is distance to terrain hit
	float st = t;
	if (t > 0.0)
	{
		vec3 pos = ro + t * rd;
		vec3 normal = calcNormal(pos, t);
		vec3 ref = reflect(rd, normal);

		vec3 bn = -1.0 + 2.0 * texcube(iChannel0, 3.0 * pos / 4.0, normal).xyz;
		normal = normalize(normal + 0.6 * bn);

		float hh = 1.0 - smoothstep(-2.0, 1.0, pos.y);

		// lighting
		float sun = clamp(dot(normal, lig), 0.0, 1.0);
		float sha = 0.0;
		if (sun > 0.01)
			sha = softshadow(pos, lig, 0.01, 32.0);
		float bac = clamp(dot(normal, normalize(lig * vec3(-1.0, 0.0, -1.0))), 0.0, 1.0);
		float sky = 0.5 + 0.5 * normal.y;
		float lav = smoothstep(0.5, 0.55, lava(0.1 * pos.xz)) * hh * clamp(0.5 - 0.5 * normal.y, 0.0, 1.0);
		float occ = pow((1.0 - displacement(pos * vec3(0.8, 1.0, 0.8))) * 1.6 - 0.5, 2.0);

		float amb = 1.0;

		col = vec3(0.8);

		vec3 lin = vec3(0.0);
		lin += 1.4 * sun * vec3(1.80, 1.27, 0.99) * pow(vec3(sha), vec3(1.0, 1.2, 1.5));
		lin += 0.9 * sky * vec3(0.16, 0.20, 0.40) * occ;
		lin += 0.9 * bac * vec3(0.40, 0.28, 0.20) * occ;
		lin += 0.9 * amb * vec3(0.15, 0.17, 0.20) * occ;
		lin += lav * vec3(3.00, 0.61, 0.00);

		// surface shading/material		
		col = texcube(iChannel1, 0.5 * pos, normal).xyz;
		col = col * (0.2 + 0.8 * texcube(iChannel2, 4.0 * vec3(2.0, 8.0, 2.0) * pos, normal).x);
		vec3 verde = vec3(1.0, 0.9, 0.2);
		verde *= texture(iChannel2, pos.xz).xyz;
		col = mix(col, 0.8 * verde, hh);

		float vv = smoothstep(0.0, 0.8, normal.y) * smoothstep(0.0, 0.1, pos.y - 0.8);
		verde = vec3(0.2, 0.45, 0.1);
		verde *= texture(iChannel2, 30.0 * pos.xz).xyz;
		verde += 0.2 * texture(iChannel2, 1.0 * pos.xz).xyz;
		vv *= smoothstep(0.0, 0.5, texture(iChannel2, 0.1 * pos.xz + 0.01 * normal.x).x);
		col = mix(col, verde * 1.1, vv);

		col = lin * col;

		// sun specularity
		vec3 hal = normalize(lig - rd);
		col += 1.0 * vec3(1.80,1.27,0.99)
				* pow(clamp(dot(normal, hal), 0.0, 1.0), 16.0)
				* sun * sha
				* (0.04 + 0.96 * pow(clamp(1.0 + dot(hal, rd), 0.0, 1.0), 5.0));

		// atmospheric
		col = mix(col, (1.0 - 0.7 * hh) * bcol, 1.0 - exp(-0.00006 * t * t * t));
	}

	// sun glow
	col += vec3(1.0, 0.6, 0.2) * 0.2 * pow(sun, 2.0) * clamp((rd.y + 0.4) / (0.0 + 0.4), 0.0, 1.0);

	// smoke
	#if SMOKE
	{
	if (t < 0.0)
		t = 600.0;
	vec4 res = raymarchClouds(ro, rd, bcol, t);
	col = mix(col, res.xyz, res.w);
	}
	#endif

	// gamma	
	col = pow(clamp(col, 0.0, 1.0), vec3(0.45));

	// contrast, desat, tint and vignetting	
	col = col * 0.3 + 0.7 * col * col * (3.0 - 2.0 * col);
	col = mix(col, vec3(col.x + col.y + col.z) * 0.33, 0.2);
	col *= 1.25 * vec3(1.02, 1.05, 1.0);

	//-------------------------------------
	// velocity vectors (through depth reprojection)
	//-------------------------------------
	float vel = -1.0;
	/*TJ: This was for motion blur, a post-render effect using per-pixel
		  velocity vectors passed via vec4[3] i.e. unused alpha channel!
	if (st > 0.0)
	{
		// old camera position
		float oldTime = time - 1.0 / 30.0;		// 1/30 of a second blur
		vec3 oldRo, oldTa;
		float oldCr, oldFl;
		moveCamera(oldTime, oldRo, oldTa, oldCr, oldFl);
		mat3 oldCam = setCamera(oldRo, oldTa, oldCr);

		// world space
		vec3 wpos = ro + rd * st;
		// camera space
		//vec3 cpos = vec3(dot(wpos - oldRo, oldCam[0]),
		//					dot(wpos - oldRo, oldCam[1]),
		//					dot(wpos - oldRo, oldCam[2]));
		vec3 cpos = (wpos - oldRo) * oldCam;

		// ndc space
		vec2 npos = oldFl * cpos.xy / cpos.z;
		// screen space
		vec2 spos = 0.5 + 0.5 * npos * vec2(ubo.iResolution.y / ubo.iResolution.x, 1.0);

		// compress velocity vector in a single float
		vec2 uv = fragCoord / ubo.iResolution.xy;
		spos = clamp(0.5 + 0.5 * (spos - uv) / 0.25, 0.0, 1.0);
		vel = floor(spos.x * 255.0) + floor(spos.y * 255.0) * 256.0;
	}*/

	fragColor = vec4(col, vel);

	// Draw box/dot at mouse pointer coordinates, just for debug purposes:
	if (ubo.iMouse.x > gl_FragCoord.x - 5 && ubo.iMouse.x < gl_FragCoord.x + 5
	 && ubo.iMouse.y > gl_FragCoord.y - 5 && ubo.iMouse.y < gl_FragCoord.y + 5)
		fragColor = vec4(0, 0, 0, 1);
}

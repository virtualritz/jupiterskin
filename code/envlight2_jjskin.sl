/******************************************************************************/
/* Copyright (c) 2009-2010 by Jupiter Jazz Ltd. All Rights Reserved.          */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*    Copyright (c)The 3Delight Team.                                         */
/*    All Rights Reserved.                                                    */
/*                                                                            */
/******************************************************************************/

/*
	A light source for image based lighting. 
	
	PARAMETERS
	light_color
              : a multiplier on the light from the envmap.
	Kenv      : intensity multiplier
	Kocc      : if >0, trace transmission rays to take into account occlusion
	            by other objects
	envmap    : the environment map to use
	envspace  : the coordinate system where to place the environment map
	samples   : number of samples to use to compute lighting. Setting samples
	            to 0 will use the SH-based indirectdiffuse approximation
	            without ray-tracing.
	bias      : ray-tracing bias.
*/

////////////////////////////////////////////////////////////////////////////////
// DNA's envlight2, with some tweaks added to allow message passing of
// scanned normals
#include "shading_utils.h"
#include "jjskin_utils.h"

light
envlight2_jjskin(
					color light_color = color(1);
					float Kenv = 1.0;
					float Kocc = 0.0;
					uniform string envmap = "", envspace = "world";
					uniform float samples = 64;
					uniform float maxdist = 1e+10;
					uniform float bias = -1;
					uniform string __category = "";
					uniform float __nondiffuse = 0, __nonspecular = 1;
					varying normal newnormals = normal(0);
					uniform float useenvlight = 0;
		)
{
	uniform string raytype = "";
	rayinfo( "type", raytype );

	uniform string gather_cat = concat( "environment:", envmap );
	vector envdir = vector(0);
	color envcolor = color(0);
	float solidangle = 0;

	if (raytype == "light" ) {
		/* This light is called to cast some photons. Use gather() to give us
		 * the most interesting directions and use a solar() construct to
		 * cast a photon coming from that direction. (The photon casting
		 * process is dealt with automagically inside 3Delight) */
		gather(	gather_cat, 0, 0, 0, samples, "environment:color", envcolor,
				"environment:direction", envdir,
				"environment:solidangle", solidangle )
		{
			envdir = vtransform( envspace, "current", envdir );
			// convert from solid angle to angle
			solar( -envdir, acos( 1 - solidangle / (_2PI) ))
			{
				Cl = envcolor * light_color / _4PI;
			}
		}
	} else {
		// read scanned normals, if useenvlight is checked, otherwise Ns
		normal shading_normal = (useenvlight == 0) ?
			ShadingNormal( normalize(Ns) ) : newnormals;

		if (samples == 0) {
			// use the SH-based approximation
			normal tN = ntransform( envspace, shading_normal );
			illuminate( Ps + shading_normal ) // shade all surface points
			{
				Cl = Kenv * light_color * indirectdiffuse( envmap, tN ) / _4PI;
			}
		} else {
			uniform float t1 = 0.025 * sqrt( samples );
			uniform float t2 = 0.050 * sqrt( samples );
			
			illuminate( Ps + shading_normal ) // shade all surface points
			{
				Cl = color(0);

				if (Kenv > 0) {
					vector raydir = vector(0);

					gather(	gather_cat, 0, 0, 0, samples,
							"environment:color", envcolor,
							"ray:direction", raydir,
							"environment:direction", envdir,
							"environment:solidangle", solidangle )
					{
						raydir = vtransform( envspace, "current", raydir );
						envdir = vtransform( envspace, "current", envdir );
						float atten = max( shading_normal.normalize(envdir),0);
						float kocc = 1 - smoothstep( t1, t2, solidangle );
						kocc *= Kocc;

						uniform float diffusedepth = 0;
						attribute( "diffusedepth", diffusedepth );

						color trs = color(1);
						if (kocc > 0 && atten > 0 && diffusedepth == 0)
						{
							trs = transmission( Ps, Ps + raydir * maxdist,
									"bias", bias );
							trs = 1 - kocc * (1 - trs);
						}
						Cl += envcolor * trs * atten * light_color / _4PI;
					}
					Cl *= Kenv;
				}
			}
		}
	}
}


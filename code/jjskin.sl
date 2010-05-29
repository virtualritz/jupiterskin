/******************************************************************************/
/* Copyright (c) 2009-2010 by Jupiter Jazz Ltd. All Rights Reserved.          */
/*                                                                            */
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/* Jupiter Jazz JJskin
 * author: Luis Barrancos (luis.b.barrancos_AT_gmail.com)
 *
 * based on:
 * "Rapid Acquisition of Specular and Diffuse Normal Maps from Polarized
 * Spherical Gradient Illumination", Eurographics Symposium on Rendering, 2007
 * by Wan-Chun Ma, Tim Hawkins, Pieter Peers, Charles-Felix Chabert,
 * Malte Weiss, Paul Debevec, University of Southern California Institute for
 * Creative Technologies
 * and:
 * "Estimating Specular Roughness and Anisotropy from Second Order Spherical
 * Gradient Illumination", Eurographics Symposium on Rendering, 2009
 * by Abhijeet Ghosh, Tongbo Chen, Pieter Peers, Cyrus A.Wilson, Paul Debevec,
 * University of Southern California Institute for Creative Technologies.
 *
 *	parameters:
 *
 * 		Ka, Kd		- ambient and diffuse coefficients
 * 		Ks1, Ks2	- first and second specular lobe's coefficients
 * 		roughness1	- specular roughness for first specular lobe
 * 		roughness2	- specular roughness for second specular lobe
 * 		ior			- index of refraction (1.4 recommended for skin)
 * 		weight		- blend factor for the two specular lobes
 * 					  0, gives full weight to the first lobe, 1 to the second
 * 		gammain		- gamma of the input textures
 * 		R/G/Bmap	- normal maps for R, G, B wavelenghts
 * 		Smap		- normal map for specular term
 * 		diffalb		- diffuse albedo map
 * 		specalb		- specular albedo map
 * 		specmap		- specular intensity map - includes roughness for the 
 * 					  first and second specular lobes in the R and G channels
 * 					  respectively, and the mixing factor between the 2 lobes
 * 					  in the B channel
 * 		blendmask	- if provided, controls the blending between the actual
 * 					  surface's normals, and the mapped normals
 * 		transmask	- if provided, controls the opacity of the shader, if not
 * 					  the value of the primitive variable Os is used
 * 		usespecmap	- if checked, the specular roughness values for both 
 * 					  specular lobes, and the mixing factor, are retrieved from
 * 					  the R, G, B channels of the specular intensity map,
 * 					  otherwise, the values in roughness1, roughness2, and
 * 					  weight are used instead.
 * 		useTSlobes	- toggle, 0 uses the Beckmann distribution (Cook-Torrance)
 *					- and 1 uses a Gaussian distribution (Torrance-Sparrow)
 * 					- distribution.
 * 		category	- light category
 * 		dobake		- if checked, the shader runs only a simple ambient and
 * 					  diffuse terms with the red normal, and bakes _radiance_t
 * 					  and _area for subsurface use (not only)
 * 		bakefile	- the point cloud file to bake, or to read for the SSS
 * 					  contribution.
 * 		ptccoordsys	- the destination coordinate system of the point cloud
 * 		Kr			- reflection amount
 * 		subset		- the geometry subset to consider for ray intersections.
 * 					- defaults to "-_3dfm_not_visible_in_reflections", so it
 * 					  considers all objects except those in the not visible to
 * 					  reflections subset.
 * 		hitsides	- sides to consider for ray intersection
 * 		envmap		- environment map to use when there are no ray hits
 * 		envspace	- coorinate system of the environment map
 * 		filter		- filter to use for the environment map lookup
 * 		bias		- bias
 * 		maxdist		- maximum distance for rays to travel before considering a
 * 					  ray miss
 * 		width		- filter width for environment lookup
 * 		raysamples	- number of samples to use for raytracing
 * 		envsamples	- number of samples for environment lookup
 * 		envblur		- amount of blur for environment lookup
 *
 * */ 
////////////////////////////////////////////////////////////////////////////////

#include "jjskin_utils.h" // useful quantities and functions
#include "jjskin_3dfm_pragmas.h"
#include "jjskin_aovs.h"

////////////////////////////////////////////////////////////////////////////////

JJSKIN_3DFM_PRAGMAS
JJSKIN_3DFM_PRAGMA_GROUPS

////////////////////////////////////////////////////////////////////////////////
surface jjskin(
		float Ka = 0.05, Kd = 0.8, Ks1 = 0.5, Ks2 = 0.5;
		float Ksscale = 1;
		float roughness1 = 0.25, roughness2 = 0.5;
		float weight = 0.5;
		float romin = 0, romax = 1, rnmin = 0, rnmax = 1;
		color ambcolor = color(0);
		// subsurface
		float Ksss = 1, scalesss = 0.1, balancesss = 0.5;
		color dmfp = MND_SKIN1, albedo = ALB_SKIN1;
		float ior = 1.4;
		float gammain = 1.0;
		// normal maps + diffuse/specular albedo + specular intensity
		uniform string	Rmap = "",
						Gmap = "",
						Bmap = "",
						Smap = "",
						diffalb = "",
						specalb = "",
						specmap = "";
		// blending mask (mapped>surface normals) and transparency
		uniform string	blendmask = "",
						transmask = "";
		uniform float usespecmap = 1, useTSlobes = 0;
		// light category
		uniform string category = "";
		// bake
		uniform float dobake = 0;
		uniform string	bakefile = "",
						ptccoordsys = "world",
						areachannel = "_area",
						radiancechannel = "_radiance_t";
		// reflection
		float Kr = 1;
		color reflcolor = color(1);
		uniform string	label = "",
						subset = "-_3dfm_not_visible_in_reflections",
						hitsides = "front",
						envmap = "",
						envspace = "world",
						filter = "gaussian";
		uniform float bias = -1, maxdist = 1e+38, width = 2;
		uniform float raysamples = 8, envsamples = 4;
		float envblur = 0.1;
		// Pref, Nref (need to exist, otherwise expect chaos)
		varying point Pref = point(1e-10);
		varying normal Nref = normal(1e-10);
		uniform float useenvlight = 0;
		// AOVs
		DEFAULT_AOV_OUTPUT_PARAMETERS
		)
{
	INIT_AOV_PARAMETERS

	// actual normals, for fallback, and to build tangent space ahead
	normal Ns = ShadingNormal( normalize(N) );

	vector Vf = -normalize(I), Ln = vector(0), Hn = vector(0);
	float ns_dot_v = Ns.Vf, ns_dot_map = 0, alpha = 0;
	normal newnormals[4] = normal(0); // R,G,B,S normals

	// transparency mask value
	aov_transparency += (transmask != "") ?
		color texture( transmask, TEXLOOKUP_PARAMETERS ) : Os;

	// need diffuse albedo, linearize if needed, if not available, use Cs
	aov_diffuse_color += clamp( (diffalb != "") ?
			( (gammain != 1) ?
			  gamma( color texture( diffalb, TEXLOOKUP_PARAMETERS ), gammain) :
			  color texture( diffalb, TEXLOOKUP_PARAMETERS) ) : Cs,
			color(0), color(1) );

	// need already a normal map for baking the PTC file. Use the red channel
	normal nnred = (dobake != 0 || Rmap != "") ?
		getnormal( normalize( normal( color texture( Rmap,
							NORMAL_TEXLOOKUP_PARAMETERS ))), Nref, Ns,
				Pref, P, s, t ) : Ns;

	// Photon macro, use the matte photon shading model
	BEGIN_JJSKIN_PHOTON_CASE( aov_diffuse_color, aov_transparency )
	// bake PTC if set, use red normal (it's already transformed via getnormal)
	DO_BAKE_BEGIN( dobake, bakefile, aov_diffuse_color, nnred )

	// storage
	color Cdiff = color(0), Cspec = color(0), ctemp = color(0);
	color reflection_raytraced = color(0), reflection_environment = color(0);

	float r1 = 0, r2 = 0, w = 0, lobe1 = 0, lobe2 = 0;

	uniform float nondiff = 0, nonspec = 0, i;

	// get specular albedo and linearize if needed, no texture, use Cs
	// clamp values just in case (seemed to be outside [0,1] in some test imgs)
	aov_specular_color += clamp( (specalb != "") ?
			( (gammain != 1) ?
			  gamma( color texture( specalb, TEXLOOKUP_PARAMETERS), gammain) :
			  color texture( specalb, TEXLOOKUP_PARAMETERS) ) : Cs,
			color(0), color(1) );

	// get (float) blend mask, to blend between surface and mapped normals
	// if needed/desired
	aov_blendfactor += clamp( (blendmask != "") ?
			float texture( blendmask, TEXLOOKUP_PARAMETERS) : 0, 0, 1 );

	// Read specular roughness for first and second specular lobe, from the
	// specular intensity's map R and G channels, and use B as a weighting
	// factor. Alternatively use the user-supplied roughness parameters
	if (usespecmap == 1 && specmap != "") {
		// allow the user to remap the roughness values.
		// In "A Spectral BSSRDF", a roughness of [0.2,0.4] is suggested
		color specinten = clamp( (gammain != 1) ?
				gamma( color texture( specmap, TEXLOOKUP_PARAMETERS), gammain) :
				color texture( specmap, TEXLOOKUP_PARAMETERS), color(0),
				color(1) );
		r1 = remap( specinten[0], romin, romax, rnmin, rnmax );
		r2 = remap( specinten[1], romin, romax, rnmin, rnmax );
		aov_lobebalance += specinten[2];
	} else { // allow remap still, user might want to connect (float) images
		r1 = remap( roughness1, romin, romax, rnmin, rnmax );
		r2 = remap( roughness2, romin, romax, rnmin, rnmax );
		aov_lobebalance += weight;
	}
	float r1sqr = 2*SQR(r1), r2sqr = 2*SQR(r2); // for TS lobes

	// do RGBS shading if maps are provided, otherwise fallback to diffuse()
	// + specular()
	if (Rmap != "" && Gmap != "" && Bmap != "" && Smap != "") {

		// normal maps should be in linear space
		uniform string normalmaps[4] = { Rmap, Gmap, Bmap, Smap };
		uniform string indexedtex = "";

		for (i = 0; i < 4; i += 1) {

			indexedtex = normalmaps[i];
			// The red normal was set earlier, and getnormal() is expensive
			// use it instead, even the tests in the loop are cheaper.
			// Blend from mapped normals to surface normals as (if) set earlier
			newnormals[i] = mix( (i == 0) ? nnred :
					/* return values are normalized, ignore warnings */
					getnormal( normalize( normal( color texture( indexedtex,
									NORMAL_TEXLOOKUP_PARAMETERS))), Nref,
						Ns, Pref, P, s, t ), Ns, aov_blendfactor );

			// Needed to check orientation
			ns_dot_map = Ns.newnormals[i];

			// Need to use scanned normals, not actual surface normals
			// If useenvlight is set, send to the (edited envlight2) light
			// shader the scanned normals, otherwise don't send a thing
			if (useenvlight == 0) {
				illuminance( category, P, newnormals[i], PI_OVER_2 )
				{
					Ln = normalize(L);
					if (i < 3) { // diffuse term
						nondiff = 0;
						// light sources need to have __nondiffuse
						if (1 == lightsource("__nondiffuse", nondiff) &&
								nondiff < 1) {
							ctemp = Cl * jjskin_diffuse( Ln, Ns, newnormals[i],
									ns_dot_v, ns_dot_map, nondiff);
							Cdiff[i] += ctemp[i];
							ctemp = color(0);
						}
					} else { // specular term
						nonspec = 0;
						// light sources need to have __nonspecular
						if (1 == lightsource("__nonspecular", nonspec) &&
								nonspec < 1) {
							// get specular term for 1st and 2nd lobes
							jjskin_specular( Ln, Vf, Ns, newnormals[i],
									ns_dot_v, ns_dot_map, Ks1, Ks2, r1, r2,
									r1sqr, r2sqr, ior, useTSlobes,
									lobe1, lobe2 );
							aov_specularlobe1 += lobe1;
							aov_specularlobe2 += lobe2;
							Cspec += Cl * (1-nonspec) * mix( lobe1, lobe2,
									aov_lobebalance );
						}
					}
				}
			} else { // envlight is set
				// send scanned normals to env light shader
				illuminance( category, P, newnormals[i], PI_OVER_2,
						"send:light:newnormals", newnormals[i],
						"send:light:useenvlight", useenvlight
						/* no need to refresh light cache */ )
				{
					Ln = normalize(L);
					if (i < 3) { // diffuse term
						nondiff = 0;
						// light sources need to have __nondiffuse
						if (1 == lightsource("__nondiffuse", nondiff) &&
								nondiff < 1) {
							ctemp = Cl * jjskin_diffuse( Ln, Ns, newnormals[i],
									ns_dot_v, ns_dot_map, nondiff);
							Cdiff[i] += ctemp[i];
							ctemp = color(0);
						}
					} else { // specular term
						nonspec = 0;
						// light sources need to have __nonspecular
						if (1 == lightsource("__nonspecular", nonspec) &&
								nonspec < 1) {
							// get specular term for 1st and 2nd lobes
							jjskin_specular( Ln, Vf, Ns, newnormals[i],
									ns_dot_v, ns_dot_map, Ks1, Ks2, r1, r2,
									r1sqr, r2sqr, ior, useTSlobes,
									lobe1, lobe2 );
							aov_specularlobe1 += lobe1;
							aov_specularlobe2 += lobe2;
							Cspec += Cl * (1-nonspec) * mix( lobe1, lobe2,
									aov_lobebalance );
						}
					}
				} // illuminance
			} // envlight check
		} // for loop
		aov_diffuse += Kd * Cdiff;
		// scale overall specular contribution
		aov_specular += clamp( Ksscale * Cspec, color(0), color(1) );
		// store mapped normals into AOVs
		aov_Rmap_normal += newnormals[0];
		aov_Gmap_normal += newnormals[1];
		aov_Bmap_normal += newnormals[2];
		aov_Smap_normal += newnormals[3];
	} else { // fallback for no texmaps, use diffuse+specular with Ns
		aov_diffuse += Kd * diffuse(Ns);
		// use Ks1 and roughness1 for overall Ks/roughness, ignore Ksscale
		aov_specular += Ks1 * specular( Ns, Vf, roughness1 );
	} // end of map checks/main lighting

	// output gamma is left to the renderer
	aov_ambient_color += ambcolor;
	aov_ambient += Ka * ambient();

	// Subsurface
	// in case the user really wants SSS, besides the subsurface effect
	// given by the RGBS normals. No single pass approach, PTC is needed
	if (bakefile != "" && Ksss != 0) {
		// subsurface term (use red normal, which is the smoothest)
		aov_subsurfacescattering += subsurface(
				/* subsurface() behaviour changed in 3delight 9.0.25,
				 * now default space is "world" to match ptc space, and
				 * subsurface() accepts coordinatesystem parameter, use it */
				P, nnred, "filename", bakefile,
				"coordsystem", ptccoordsys,
				"areachannel", areachannel,
				"radiancechannel", radiancechannel,
				/* varying albedo is supported, use diffuse albedo if
				 * SSS albedo parameter is 0, otherwise use the parameter
				 * value instead */
				"diffusemeanfreepath", dmfp, "albedo",
				(albedo == 0) ? aov_diffuse_color : albedo,
				"scale", scalesss, "ior", ior ) * Ksss;

		// balance between diffuse and subsurface terms, in any case, AOVs
		// are given
		aov_sssmixfactor += balancesss;
		aov_diffusesssblend += mix( aov_diffuse * aov_diffuse_color,
				aov_subsurfacescattering, aov_sssmixfactor );
	} else { // need to set diffusesssblend properly without SSS
		aov_diffusesssblend += aov_diffuse * aov_diffuse_color;
	}

	// Reflection
	aov_reflection_color += reflcolor;
	if (Kr != 0) {
		// The specular normal will be used for Fresnel and reflections
		aov_reflection_fresnel += schlickfresnel( Vf.newnormals[3], ior );
		// Raytraced, and environment reflection values aren't modulated by
		// the Fresnel reflection factor. The user can do this in post if
		// desired, since it also has more freedom to tweak the intensity of
		// the reflection. The combined reflection AOV however, takes the
		// Fresnel reflection amount into considerations.
		// Normals are normalized, ignore warnings
		aov_reflection += Kr * get_reflection( -Vf, newnormals[3], r1,
				raysamples, envsamples, bias, maxdist, envblur, width,
				label, subset, hitsides, envmap, envspace,
				reflection_raytraced, reflection_environment ) *
			aov_reflection_fresnel;
		aov_reflection_raytraced += reflection_raytraced;
		aov_reflection_environment += reflection_environment;
	}

	// finally
	aov_Oi += aov_transparency;
	aov_Ci += aov_transparency * (
			( aov_ambient_color * aov_ambient ) +
			  aov_diffusesssblend +
			( aov_specular_color * aov_specular ) +
			( aov_reflection_color * aov_reflection )
			);
	// close bake and photon macros
	DO_BAKE_END
	END_JJSKIN_PHOTON_CASE

	// The macro gets 3DfM's (output varying float) __3dfm_shadowing, but the
	// symbol needs to be set
#ifndef USE_AOV_aov_shadow
#define USE_AOV_aov_shadow
	GETSHADOWS( category, P )
#endif // USE_AOV_aov_shadow

	Oi = aov_Oi;
	Ci = aov_Ci;
}


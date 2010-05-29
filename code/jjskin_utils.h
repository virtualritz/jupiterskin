/******************************************************************************/
/* Copyright (c) 2009-2010 by Jupiter Jazz Ltd. All Rights Reserved.          */
/*                                                                            */
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/* Jupiter Jazz JJskin
 * author: Luis Barrancos (luis.b.barrancos_AT_gmail.com)
 *
 * set of utilities and useful constants for main JJskin
 * */

////////////////////////////////////////////////////////////////////////////////
// 3Delight for Maya's shading_utils.h, for BEGIN/END_PHOTON_CASE macros,
// and ShadingNormal(). Need to set AOVs accordingly later.
// ( include/maya/rsl )
#ifndef JJSKIN_UTILS_H
#define JJSKIN_UTILS_H	1

#include "shading_utils.h"

////////////////////////////////////////////////////////////////////////////////
// Normal maps lookup parameters
#define NORMAL_TEXLOOKUP_PARAMETERS \
		s, t, s, t, s, t, s, t, "filter", "box", "samples", 1

// texture lookup parameters (easier to set for all the lookups in main)
#define TEXLOOKUP_PARAMETERS \
		s, t, "filter", "gaussian", "blur", 0

// ptc bake macro
#define DO_BAKE_BEGIN( DOBAKE, BAKEFILE, DIFFCOLOR, NS ) \
		if (DOBAKE == 1 && BAKEFILE != "") { \
			aov_ambient += Ka * ambient(); \
			aov_diffuse += Kd * diffuse(NS); \
			aov_Ci = DIFFCOLOR * (aov_ambient + aov_diffuse) + \
				subsurface(P); \
			float A = area(P); \
			bake3d( bakefile, "", P, NS, "_radiance_t", aov_Ci, \
					"_area", A, "interpolate", 1 ); \
		} else {

// :ahem:
#define DO_BAKE_END }

// need a minimalistic version of 3dfm's photon macro
// ideally one would use AOVs here
#define BEGIN_JJSKIN_PHOTON_CASE( DIFFUSECOLOR, TRANSPARENCY ) \
	uniform string raytype = ""; \
	if (1 == rayinfo("type", raytype) && raytype == "light") { \
		aov_Ci += DIFFUSECOLOR; \
		aov_Oi += TRANSPARENCY; \
	} else {

#define END_JJSKIN_PHOTON_CASE	} // close previous brace

// useful quantities (just in case, check if definitions exist)
#ifndef PI_OVER_2
#define PI_OVER_2	1.570796326794896
#endif

#ifndef _2PI
#define _2PI		6.283185307179586
#endif

#ifndef _4PI
#define _4PI		12.56637061435917
#endif

#ifndef EPS
#define EPS			0.00001
#endif

#ifndef SQR
#define SQR(X)		( (X) * (X) )
#endif

// Skin data from "A Practical Model for Subsurface Light Transport", by
// Henrik Wann Jensen, Steve Marschner, Marc Levoy, Pat Hanrahan
// mean scattering depth = 1 / sqrt(3*(sigma_s+sigma_a)*sigma_a)
// sigma_s and a, scattering and absorption coefficients respectively
#define MND_SKIN1	color( 3.673294, 1.366533, 0.682693 )
#define ALB_SKIN1	color( 0.440000, 0.220000, 0.130000 )

#define MND_SKIN2	color( 4.821475, 1.693699, 1.089970 )
#define ALB_SKIN2	color( 0.630000, 0.440000, 0.340000 )

////////////////////////////////////////////////////////////////////////////////
// Gamma controls (bias and gain might be added later)
float gamma( float x, gamma; ) { return pow( x, 1/gamma); }

color gamma( color x; float gamma; ) {
	return color(	float gamma( x[0], gamma),
					float gamma( x[1], gamma),
					float gamma( x[2], gamma) );
}

float remap( float value, romin, romax, rnmin, rnmax; )
{
	float scale = (value - romin) / (romax-romin);
	float out = (rnmin < rnmax) ?
		rnmin + scale * (rnmax - rnmin) : rnmin - scale * (rnmin - rnmax);
	return out;
}

////////////////////////////////////////////////////////////////////////////////
// transform scanned normals
normal getnormal(
					normal Nscanned, Nref, Ns;
					point Pref, PP;
					float ss, tt;
					)
{
	// build TBN matrix of reference mesh
	normal Nsref = normalize(Nref); // it wasn't normalized when passed
	// build tangent along S direction
	float det = Du(ss)*Dv(tt) - Dv(ss)*Du(tt);
	vector Tn = normalize( (Du(Pref)*Dv(tt) - Dv(Pref)*Du(tt)) / det );
	vector Bn = normalize( Tn^Nsref ); // bitangent
	Tn = normalize( Nsref^Bn ); // tangent
	// in renderman, vector premultiplication, build TBN matrix of ref mesh
	matrix TBNref = matrix(	Tn[0], Bn[0], Nsref[0], 0,
							Tn[1], Bn[1], Nsref[1], 0,
							Tn[2], Bn[2], Nsref[2], 0,
							0, 0, 0, 1 );
	// transform scanned normals into tangent space of reference mesh
	// use "shader" fromspace, Pref and Nref shouldn't be in world or current
	normal Ntangent = normalize( transform( "shader", TBNref, Nscanned ) );

	// build TBN matrix of deformed mesh
	// NS = ShadingNormal( normalize(N) );
	Tn = normalize( (Du(PP)*Dv(tt) - Dv(PP)*Du(tt)) / det );
	Bn = normalize( Tn^Ns );
	Tn = normalize( Ns^Bn );
	// transpose
	matrix TBN = matrix(	Tn[0], Tn[1], Tn[2], 0,
							Bn[0], Bn[1], Bn[2], 0,
							Ns[0], Ns[1], Ns[2], 0,
							0, 0, 0, 1 );
	// transform tangent space normals back via TBN matrix of deformed mesh
	normal Nnew = normalize( transform( "camera", TBN, Ntangent ) );

	return Nnew;
}

////////////////////////////////////////////////////////////////////////////////
// Christophe Schlick's fast Fresnel approximation, for rays entering
// denser medium, without total internal reflection.
float schlickfresnel(	float costheta, ior; )
{
	float kr = (ior - 1) / (ior + 1);
	kr *= kr;
	return kr + (1 - kr) * pow(1 - costheta, 5);
}

////////////////////////////////////////////////////////////////////////////////
// Beckmann microfacet distribution function
// For the microfacet distribution and geometric masking/shadowing terms, also
// see the approximation suggested by Csaba Kelemen, Laszlo Szirmay-Kalos,
// in "A microfacet based coupled specular-matte BRDF model with importance
// sampling", or the approximation for the first equation of the Smith
// shadowing/masking function for a known microfacet normal, in
// "Microfacet models for refraction through rough surfaces", by Bruce Walter,
// Stephen R.Marschner, Hongsong Li, Kenneth E.Torrance, Program of Computer
// Graphics, Cornell University, Beijing Institute of Technology, 3,6-7.

float beckmann(	float cosalpha, roughness; )
{
	// (1/ cos^2(alpha) m^2) * exp( -(tan^2(alpha)/m^2)
	float m2 = SQR(roughness);
	float c2 = SQR(cosalpha);
	return exp( (c2-1) / (c2*m2)) / ( SQR(c2)*m2);
}

////////////////////////////////////////////////////////////////////////////////
// Geometric masking/shadowing (Torrance-Sparrow). /////////////////////////////
float geomasking( float cos_theta_r, cos_alpha, cos_theta_i, cos_beta; )
{
	float shadowing = (cos_alpha * cos_theta_r) / cos_beta;
	float masking = (cos_alpha * cos_theta_i) / cos_beta;
	return min(1, 2 * min( shadowing, masking));
}

////////////////////////////////////////////////////////////////////////////////
// Diffuse and specular terms. /////////////////////////////////////////////////
// If the actual surface normal is visible to viewer but angle between
// scanned normal and real surface normal > 90', use surface normal instead.
// This helps reduce artifacts in areas with small details with random-ish
// orientation (eyelashes, beard...)

color
jjskin_diffuse(
				vector Ln;
				normal Ns, ScannedNormal;
				float ns_dot_v, ns_dot_map;
				uniform float nondiff;
		)
{
	float cos_theta_i = (ns_dot_v >= 0 && ns_dot_map <= 0) ?
		Ln.Ns : Ln.ScannedNormal;
	return ((1 - nondiff) * cos_theta_i);
}

////////////////////////////////////////////////////////////////////////////////
// Specular term. //////////////////////////////////////////////////////////////
// The captured specular roughness isn't directly tied to any particular
// analytical BRDF. In any case, the best results are obtained with spatially
// varying Torrance-Sparrow lobes (the paper mentions Beckmann distribution,
// see "Analysis of Human Faces using a Measurement-Based Skin Reflectance
// Model", by Tim Weyrich, Wojciech Matusik, Hanspeter Pfister, Bernd Bickel,
// Craig Donner, Chien Tu, Jinho Lee, Janet McAndless, Addy Ngan,Markus Gross,
// Henrik Wann Jensen.

void
jjskin_specular(
					vector Ln, Vf;
					normal Ns, ScannedNormal;
					float ns_dot_v, ns_dot_map;
					float Ks1, Ks2, r1, r2, r1sqr, r2sqr, ior;
					uniform float useTSlobes;
					output varying float lobe1, lobe2;
		)
{
	float cos_theta_i = 0, cos_theta_r = 0, cos_alpha = 0, cos_beta = 0;
	float alpha = 0, D = 0, G = 0, F = 0;
	vector Hn = normalize(Ln+Vf);
	// check orientation, use Ns if needed
	if (ns_dot_v >= 0 && ns_dot_map <= 0) {
		cos_theta_r = ns_dot_v;
		cos_theta_i = Ln.Ns;
		cos_alpha = Hn.Ns;
	} else {
		cos_theta_r = max( EPS, Vf.ScannedNormal );
		cos_theta_i = Ln.ScannedNormal;
		cos_alpha = Hn.ScannedNormal;
	}
	cos_beta = Ln.Hn; // = Vf.Hn

	if (useTSlobes == 0) { // use Beckmann distribution
		// 1st lobe
		D = beckmann( cos_alpha, r1 );
		G = geomasking( cos_theta_r, cos_alpha, cos_theta_i, cos_beta );
		F = schlickfresnel( cos_beta, ior );
		lobe1 = Ks1 * (D*G*F / cos_theta_r );
		// 2nd lobe
		D = beckmann( cos_alpha, r2 );
		lobe2 = Ks2 * (D*G*F / cos_theta_r );
	} else {
		// use a normal distribution. See also: "Surface Reflection: Physical
		// and Geometrical Perspectives", by Shree K.Nayaar, Katsushi Ikeuchi,
		// Takeo Kanade
		alpha = acos( cos_alpha );
		D = exp( -( SQR(alpha) / r1sqr ));
		G = geomasking( cos_theta_r, cos_alpha, cos_theta_i, cos_beta );
		F = schlickfresnel( cos_beta, ior );
		lobe1 = Ks1 * (D*G*F / cos_theta_r );
		D = exp( -( SQR(alpha) / r2sqr ));
		lobe2 = Ks2 * (D*G*F / cos_theta_r );
	}
}

////////////////////////////////////////////////////////////////////////////////
color
get_reflection(
				vector In;
				normal Ns;
				float samplecone;
				uniform float samples, envsamples, bias, maxdist;
				float envblur;
				uniform float width;
				uniform string label, subset, hitsides, envmap, envspace;
				output varying color reflection_raytraced;
				output varying color reflection_environment;
		)
{
	color hitc = color(0), cout = color(0);
	color crayhit = color(0), craymiss = color(0);
	vector R = vector(0);
	uniform float nhitr = 0, nhite = 0;
	extern point P;

	/* The normal used is the normal corresponding to the specular normal
	 * map. In any case, using too many samples, or high maximum ray distance
	 * might be a bit of an overkill, environment mapping would do just fine,
	 * and be believable enough. */
	uniform string raytype = "none";
	rayinfo("type", raytype);

	if (raytype == "camera" || raytype == "specular" ) {

		// cut down samples at higher ray depths
		uniform float raydepth = 0;
		rayinfo("depth", raydepth);
		uniform float rsamples = (raydepth > 1) ?
			max( 1, samples / pow( 2, raydepth)) : samples;

		float idotn = In.Ns; // In = normalize(I)
		if (idotn < 0) { // when visible

			if (envmap != "") { // use environment map on ray misses

				gather( "illuminance", P, reflect( In, Ns ), samplecone,
						rsamples, "distribution", "cosine",
						"surface:Ci", hitc, "ray:direction", R,
						"label", label, "subset", subset, "bias", bias,
						"raytype", "specular", "hitsides", hitsides,
						"maxdist", maxdist )
				{
					crayhit += hitc;
					nhitr += 1;
				} else {
					craymiss += environment( envmap, transform( envspace, R),
							"blur", envblur, "width", width,
							"filter", "gaussian", "samples", envsamples );
					nhite += 1;
				}
			} else { // no env lookup on ray misses
				gather( "illuminance", P, reflect( In, Ns ), samplecone,
						rsamples, "distribution", "cosine",
						"surface:Ci", hitc, "label", label,
						"subset", subset, "bias", bias,
						"raytype", "specular", "hitsides", hitsides,
						"maxdist", maxdist)
				{
					crayhit += hitc;
					nhitr += 1;
				}
			}
			reflection_raytraced = (crayhit / nhitr);
			reflection_environment = (craymiss / nhite);
			cout = reflection_raytraced + reflection_environment;
		}
	}
	return cout;
}

////////////////////////////////////////////////////////////////////////////////
#endif // JJSKIN_UTILS_H


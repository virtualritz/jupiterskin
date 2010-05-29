/******************************************************************************/
/* Copyright (c) 2009-2010 by Jupiter Jazz Ltd. All Rights Reserved.          */
/*                                                                            */
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// Set of useful AOVs for JupiterJazz JJskin shader. These although functional
// are meant as an suggestion, or departure point to further tweak this into
// your own workflow.

#ifndef JJSKIN_AOV_H
#define JJSKIN_AOV_H

#define DECLARE_AOV_PARAMETERS		\
		aov_ambient_color,			\
		aov_diffuse_color,			\
		aov_specular_color,			\
		aov_reflection_color,		\
		aov_ambient,				\
		aov_diffuse,				\
		aov_specular,				\
		aov_specularlobe1,			\
		aov_specularlobe2,			\
		aov_lobebalance,			\
		aov_reflection,				\
		aov_reflection_raytraced,	\
		aov_reflection_environment,	\
		aov_reflection_fresnel,		\
		aov_indirectdiffuse,		\
		aov_subsurfacescattering,	\
		aov_sssmixfactor,			\
		aov_diffusesssblend,		\
		aov_shadowing,				\
		aov_occlusion,				\
		aov_transparency,			\
		aov_blendfactor,			\
		aov_surfacenormal,			\
		aov_Rmap_normal,			\
		aov_Gmap_normal,			\
		aov_Bmap_normal,			\
		aov_Smap_normal,			\
		aov_Oi,						\
		aov_Ci

#define INIT_AOV_PARAMETERS \
		aov_ambient_color				= color(0);		\
		aov_diffuse_color				= color(0);		\
		aov_specular_color				= color(0);		\
		aov_reflection_color			= color(0);		\
		aov_ambient						= color(0);		\
		aov_diffuse						= color(0);		\
		aov_specular					= color(0);		\
		aov_specularlobe1				= 0;			\
		aov_specularlobe2				= 0;			\
		aov_lobebalance					= 0;			\
		aov_reflection					= color(0);		\
		aov_reflection_raytraced		= color(0);		\
		aov_reflection_environment		= color(0);		\
		aov_reflection_fresnel			= 0;			\
		aov_indirectdiffuse				= color(0);		\
		aov_subsurfacescattering		= color(0);		\
		aov_sssmixfactor				= 0;			\
		aov_diffusesssblend				= color(0);		\
		aov_shadowing					= 0;			\
		aov_occlusion					= 0;			\
		aov_transparency				= color(0);		\
		aov_blendfactor					= 0;			\
		aov_surfacenormal				= normal(0);	\
		aov_Rmap_normal					= normal(0);	\
		aov_Gmap_normal					= normal(0);	\
		aov_Bmap_normal					= normal(0);	\
		aov_Smap_normal					= normal(0);	\
		aov_Oi							= color(0);		\
		aov_Ci							= color(0);

#define DECLARE_AOV_OUTPUT_PARAMETERS					\
		output varying color aov_ambient_color;			\
		output varying color aov_diffuse_color;			\
		output varying color aov_specular_color;		\
		output varying color aov_reflection_color;		\
		output varying color aov_ambient;				\
		output varying color aov_diffuse;				\
		output varying color aov_specular;				\
		output varying float aov_specularlobe1;			\
		output varying float aov_specularlobe2;			\
		output varying float aov_lobebalance;			\
		output varying color aov_reflection;			\
		output varying color aov_reflection_raytraced;	\
		output varying color aov_reflection_environment;\
		output varying float aov_reflection_fresnel;	\
		output varying color aov_indirectdiffuse;		\
		output varying color aov_subsurfacescattering;	\
		output varying float aov_sssmixfactor;			\
		output varying color aov_diffusesssblend;		\
		output varying float aov_shadowing;				\
		output varying float aov_occlusion;				\
		output varying color aov_transparency;			\
		output varying float aov_blendfactor;			\
		output varying normal aov_surfacenormal;		\
		output varying normal aov_Rmap_normal;			\
		output varying normal aov_Gmap_normal;			\
		output varying normal aov_Bmap_normal;			\
		output varying normal aov_Smap_normal;			\
		output varying color aov_Oi;					\
		output varying color aov_Ci;					

#define DEFAULT_AOV_OUTPUT_PARAMETERS								\
		output varying color aov_ambient_color			= color(0); \
		output varying color aov_diffuse_color			= color(0); \
		output varying color aov_specular_color			= color(0); \
		output varying color aov_reflection_color		= color(0); \
		output varying color aov_ambient				= color(0); \
		output varying color aov_diffuse				= color(0); \
		output varying color aov_specular				= color(0); \
		output varying float aov_specularlobe1			= 0;		\
		output varying float aov_specularlobe2			= 0;		\
		output varying float aov_lobebalance			= 0;		\
		output varying color aov_reflection				= color(0); \
		output varying color aov_reflection_raytraced	= color(0); \
		output varying color aov_reflection_environment	= color(0); \
		output varying float aov_reflection_fresnel		= 0;		\
		output varying color aov_indirectdiffuse		= color(0); \
		output varying color aov_subsurfacescattering	= color(0); \
		output varying float aov_sssmixfactor			= 0;		\
		output varying color aov_diffusesssblend		= color(0); \
		output varying float aov_shadowing				= 0;		\
		output varying float aov_occlusion				= 0;		\
		output varying color aov_transparency			= color(0);	\
		output varying float aov_blendfactor			= 0;		\
		output varying normal aov_surfacenormal 		= normal(0);\
		output varying normal aov_Rmap_normal			= normal(0);\
		output varying normal aov_Gmap_normal			= normal(0);\
		output varying normal aov_Bmap_normal			= normal(0);\
		output varying normal aov_Smap_normal			= normal(0);\
		output varying color aov_Oi						= color(0);	\
		output varying color aov_Ci						= color(0);

// Light sources need __3dfm_shadowing, main body sets symbol to retrieve
// this value from shadow's AOV respectively.
#define GETSHADOWS( CATEGORY, PP)									\
	varying float inshadow = 0;										\
		illuminance( CATEGORY, PP ) {								\
		if (1 == lightsource("__3dfm_shadowing", inshadow) ) {		\
			aov_shadowing += (1 - inshadow);						\
			}														\
		}
////////////////////////////////////////////////////////////////////////////////
#endif // JJSKIN_AOV_H


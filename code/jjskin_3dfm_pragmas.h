/******************************************************************************/
/* Copyright (c) 2009-2010 by Jupiter Jazz Ltd. All Rights Reserved.          */
/*                                                                            */
/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
/* Jupiter Jazz JJskin
 * author: Luis Barrancos (luis.b.barrancos_AT_gmail.com)
 * */
////////////////////////////////////////////////////////////////////////////////
// set of definitions for 3Delight for Maya's UI
#ifndef JJSKIN_3DFM_PRAGMAS_H
#define JJSKIN_3DFM_PRAGMAS_H	1

#define JJSKIN_3DFM_PRAGMAS \
	_Pragma("annotation Ka \"gadgettype=floatslider;min=0;max=1;hint=Ambient coefficient\"" ) \
	_Pragma("annotation Kd \"gadgettype=floatslider;min=0;max=1;hint=Diffuse coefficient\"" ) \
	_Pragma("annotation Ks1 \"gadgettype=floatslider;min=0;max=1;hint=Specular coefficient for the first specular lobe\"" ) \
	_Pragma("annotation Ks2 \"gadgettype=floatslider;min=0;max=1;hint=Specular coefficient for the second specular lobe\"" ) \
	_Pragma("annotation Ksscale \"gadgettype=floatfield;hint=Scaling factor for specular highlights\"" ) \
	_Pragma("annotation roughness1 \"gadgettype=floatslider;min=0;max=1;hint=Specular roughness for the first specular lobe\"" ) \
	_Pragma("annotation roughness2 \"gadgettype=floatslider;min=0;max=1;hint=Specular roughness for the second specular lobe\"" ) \
	_Pragma("annotation ior \"gadgettype=floatfield;min=1;max=3;hint=Index of refraction, roughly 1.4 for skin\"" ) \
	_Pragma("annotation weight \"gadgettype=floatslider;min=0;max=1;hint=Specular lobes blending factors, 0 gives full weight to first lobe, 1 to second\"" ) \
	_Pragma("annotation diffalb \"gadgettype=inputfile;hint=Diffuse albedo map\"" ) \
	_Pragma("annotation specalb \"gadgettype=inputfile;hint=Specular albedo map\"" ) \
	_Pragma("annotation specmap \"gadgettype=inputfile;hint=Specular intensity map, might have specular roughness for first and second specular lobes in R and G channel, and mixing factor in B channel\"" ) \
	_Pragma("annotation blendmask \"gadgettype=inputfile;hint=Mask file to blend from mapped normals to surface normals;\"" ) \
	_Pragma("annotation transmask \"gadgettype=inputfile;hint=Transparency mask;\"" ) \
	_Pragma("annotation gammain \"gadgettype=floatfield;hint=Input gamma for diffuse and specular maps\"" ) \
	\
	_Pragma("annotation Rmap \"gadgettype=inputfile;hint=R wavelenght normal map\"" ) \
	_Pragma("annotation Gmap \"gadgettype=inputfile;hint=G wavelenght normal map\"" ) \
	_Pragma("annotation Bmap \"gadgettype=inputfile;hint=B wavelenght normal map\"" ) \
	_Pragma("annotation Smap \"gadgettype=inputfile;hint=Specular normal map\"" ) \
	\
	_Pragma("annotation usespecmap \"gadgettype=checkbox;hint=Toggle between using user-supplied roughness parameters, or values from specular map\"" ) \
	_Pragma("annotation useTSlobes \"gadgettype=checkbox;hint=Toggle between using the Beckmann microfacet distribution, or a Gaussian distribution\"" ) \
	_Pragma("annotation romin \"gadgettype=floatslider;min=0;max=1;hint=roughness old minimum\"" ) \
	_Pragma("annotation romax \"gadgettype=floatslider;min=0;max=1;hint=roughness old maximum\"" ) \
	_Pragma("annotation rnmin \"gadgettype=floatslider;min=0;max=1;hint=roughness new minimum\"" ) \
	_Pragma("annotation rnmax \"gadgettype=floatslider;min=0;max=1;hint=roughness new maximum\"" ) \
	_Pragma("annotation ambcolor \"gadgettype=colorslider;hint=Ambient color\"" ) \
	\
	_Pragma("annotation category \"gadgettype=textfield;hint=Light categories\"" ) \
	_Pragma("annotation useenvlight \"gadgettype=checkbox;hint=Use environment lights on/off\"" ) \
	_Pragma("annotation Ksss \"gadgettype=floatslider;min=0;max=1;hint=SSS amount\"" ) \
	_Pragma("annotation scalesss \"gadgettype=floatslider;hint=SSS scale;\"") \
	_Pragma("annotation balancesss \"gadgettype=floatslider;min=0;max=1;hint=SSS weight factor, 0 = no SSS, full diffuse term, 1 = no diffuse term and full SSS;\"") \
	_Pragma("annotation dmfp \"gadgettype=colorslider;hint=Diffuse mean free path\"" ) \
	_Pragma("annotation albedo \"gadgettype=colorslider;hint=Albedo\"" ) \
	_Pragma("annotation dobake \"gadgettype=checkbox;hint=Bake pointcloud for SSS\"" ) \
	_Pragma("annotation bakefile \"gadgettype=inputfile;hint=Point cloud file to read/bake\"" ) \
	_Pragma("annotation ptccoordsys \"gadgettype=textfield;hint=Pointcloud destination coordinate system (world)\"" ) \
	_Pragma("annotation areachannel \"gadgettype=textfield;hint=Channel name for micropolygon area, defaults to _area\"" ) \
	_Pragma("annotation radiancechannel \"gadgettype=textfield;hint=Channel name for radiance, defaults to _radiance_t\"" ) \
	_Pragma("annotation Kr \"gadgettype=floatslider;min=0;max=1;hint=Reflection amount\"") \
	_Pragma("annotation reflcolor \"gadgettype=colorslider;hint=Reflection color\"" ) \
	_Pragma("annotation envmap \"gadgettype=inputfile;hint=Environment map to lookup when there are no ray hits;\"" ) \
	_Pragma("annotation envspace \"gadgettype=textfield;hint=Environment coordinate system;\"") \
	_Pragma("annotation label \"gadgettype=textfield;hint=Ray label;\"") \
	_Pragma("annotation subset \"gadgettype=textfield;hint=Ray subset;\"") \
	_Pragma("annotation hitsides \"gadgettype=optionmenu:front:back:both;hint=Hit sides;\"") \
	_Pragma("annotation filter \"gadgettype=optionmenu:box:triangle:gaussian;hint=Environment lookup filter;\"") \
	_Pragma("annotation raysamples \"gadgettype=intfield;hint=Ray samples;\"") \
	_Pragma("annotation envsamples \"gadgettype=intfield;hint=Environment lookup samples;\"") \
	_Pragma("annotation width \"gadgettype=floatfield;hint=Environment lookup filter width;\"") \
	_Pragma("annotation envblur \"gadgettype=floatfield;hint=Environment lookup blur;\"") \
	_Pragma("annotation bias \"gadgettype=floatfield;hint=Bias;\"") \
	_Pragma("annotation maxdist \"gadgettype=floatfield;hint=Maximum distance to consider ray intersections;\"") \
	/* outputs (hidden... there has to be a better way to do this)*/ \
	_Pragma("annotation aov_ambient_color \"hide=true;\"") \
	_Pragma("annotation aov_diffuse_color \"hide=true;\"") \
	_Pragma("annotation aov_specular_color \"hide=true;\"") \
	_Pragma("annotation aov_reflection_color \"hide=true;\"") \
	_Pragma("annotation aov_ambient \"hide=true;\"") \
	_Pragma("annotation aov_diffuse \"hide=true;\"") \
	_Pragma("annotation aov_specular \"hide=true;\"") \
	_Pragma("annotation aov_specularlobe1 \"hide=true;\"") \
	_Pragma("annotation aov_specularlobe2 \"hide=true;\"") \
	_Pragma("annotation aov_lobebalance \"hide=true;\"") \
	_Pragma("annotation aov_reflection \"hide=true;\"") \
	_Pragma("annotation aov_reflection_raytraced \"hide=true;\"" ) \
	_Pragma("annotation aov_reflection_environment \"hide=true;\"" ) \
	_Pragma("annotation aov_reflection_fresnel \"hide=true;\"" ) \
	_Pragma("annotation aov_indirectdiffuse \"hide=true;\"" ) \
	_Pragma("annotation aov_subsurfacescattering \"hide=true;\"" ) \
	_Pragma("annotation aov_sssmixfactor \"hide=true;\"" ) \
	_Pragma("annotation aov_diffusesssblend \"hide=true;\"" ) \
	_Pragma("annotation aov_shadowing \"hide=true;\"" ) \
	_Pragma("annotation aov_occlusion \"hide=true;\"" ) \
	_Pragma("annotation aov_transparency \"hide=true;\"" ) \
	_Pragma("annotation aov_blendfactor \"hide=true;\"" ) \
	_Pragma("annotation aov_surfacenormal \"hide=true;\"" ) \
	_Pragma("annotation aov_Rmap_normal \"hide=true;\"" ) \
	_Pragma("annotation aov_Gmap_normal \"hide=true;\"" ) \
	_Pragma("annotation aov_Bmap_normal \"hide=true;\"" ) \
	_Pragma("annotation aov_Smap_normal \"hide=true;\"" ) \
	_Pragma("annotation aov_Oi \"hide=true;\"" ) \
	_Pragma("annotation aov_Ci \"hide=true;\"") \
	_Pragma("annotation Pref \"hide=true;\"") \
	_Pragma("annotation Nref \"hide=true;\"")

#define JJSKIN_3DFM_PRAGMA_GROUPS \
	_Pragma("annotation \"grouping\" \"Normal_maps/Rmap;\"") \
	_Pragma("annotation \"grouping\" \"Normal_maps/Gmap;\"") \
	_Pragma("annotation \"grouping\" \"Normal_maps/Bmap;\"") \
	_Pragma("annotation \"grouping\" \"Normal_maps/Smap;\"") \
	_Pragma("annotation \"grouping\" \"Texture_maps/diffalb;\"") \
	_Pragma("annotation \"grouping\" \"Texture_maps/specalb;\"") \
	_Pragma("annotation \"grouping\" \"Texture_maps/specmap;\"") \
	_Pragma("annotation \"grouping\" \"Texture_maps/blendmask;\"") \
	_Pragma("annotation \"grouping\" \"Texture_maps/transmask;\"") \
	_Pragma("annotation \"grouping\" \"Texture_maps/gammain;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Ka;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/ambcolor;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Kd;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Lobe1/Ks1;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Lobe1/roughness1;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Lobe2/Ks2;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Lobe2/roughness2;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Ksscale;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/weight;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/ior;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Remap/romin;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Remap/romax;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Remap/rnmin;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/Remap/rnmax;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/usespecmap;\"") \
	_Pragma("annotation \"grouping\" \"BRDF/useTSlobes;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/Ksss;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/dmfp;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/albedo;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/scalesss;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/balancesss;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/dobake;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/bakefile;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/areachannel;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/radiancechannel;\"") \
	_Pragma("annotation \"grouping\" \"BSSRDF/ptccoordsys;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/Kr;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/reflcolor;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/envmap;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/envspace;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/envsamples;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/label;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/subset;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/hitsides;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/raysamples;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/maxdist;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/bias;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/envblur;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/filter;\"") \
	_Pragma("annotation \"grouping\" \"Reflection/width;\"") \
	_Pragma("annotation \"grouping\" \"General/category;\"") \
	_Pragma("annotation \"grouping\" \"General/useenvlight;\"") 

////////////////////////////////////////////////////////////////////////////////
#endif // JJSKIN_3DFM_PRAGMAS_H


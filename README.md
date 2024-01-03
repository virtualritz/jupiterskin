> This is a copy of the original `/*jupiter jazz*/` Google Code repository with updated README (2023).

# Jupiter SGI Skin

*Jupiter Spherical Gradient Illumination Skin* is a
[RSL](http://en.wikipedia.org/wiki/Shading_language#RenderMan_Shading_Language) shader,
to be used with *legacy, RenderMan-compliant version* of the [3Delight renderer](http://www.3delight.com/). The shader reads scanned RGB world space normal maps for three different wavelengths (red, green, blue), and a fourth normal map to be used in the specular term.
 
> Note that *contemporary 3Delight|NSI* uses [OSL](https://en.wikipedia.org/wiki/Open_Shading_Language), a different shading language dialect. I.e. this shader will no longer work *as-is*.

This technique was [developed at USC](https://archive.is/05uO8) and allows the user to re-create the appear of the subtle *subsurface effects* of skin, together with *self-shadowing* and *inter-reflections*.

## Required Texture Maps

* Three normal maps for the *Lambertian* diffuse term contain increasing detail as the light wavelength decreases.

  * The normal map for the blue wavelength contains more detail than the normal map for the green wavelength, which in turn contains more detail than the red wavelength map.
    
* The specular normal map is used to drive the specular contribution (and the reflections), with the specular term being comprised of two specular lobes based on the *Torrance-Sparrow* model.

## Microfacet Distributions

The user has the possibility of selecting between two microfacet distribution functions used:

* the *Beckmann microfacet distribution*, as suggested in the publication by Robert L. Cook and Kenneth E. Torrance, "A reflectance model for computer graphics".
  
* a *Gaussian distribution*, which is cheaper to compute and produces slightly softer results.

The weight of each of the specular lobe's contribution can also be controlled directly by the user, or it can be derived automatically, via the extracted data in the specular intensity map.

## Point-Based Subsurface Scattering Term

This shader provides an optional subsurface term for greater flexibility.

3Delight calculates this from a point-based pre-pass that is triggered automatically of the term is nonzero. 

## Acknowledgements

This work was sponsored by [Srinivas M. Mohan](https://en.wikipedia.org/wiki/V._Srinivas_Mohan)/Indian Artists Computer Graphics Pvt. Ltd. for the movie [*Endhiran*](http://en.wikipedia.org/wiki/Endhiran), and it is based on:

### Literature

* [**Rapid Acquisition of Specular and Diffuse Normal Maps from Polarized Spherical Gradient Illumination**](http://gl.ict.usc.edu/Research/FaceScanning/) by Wan Chun Ma, Tim Hawkins, Pieter Peers, Charles-Felix Chabert, Malte Weiss, Paul Debevec -- University of Southern California Institute for Creative Technologies -- Eurographics Symposium on Rendering 2007.
  
* [**Estimating Specular Roughness and Anisotropy from Second Order Spherical Gradient Illumination**](http://gl.ict.usc.edu/Research/SpecularRoughness/) by Abhijeet Ghosh, Tongbo Chen, Pieters Peers, Cyrus A. Wilson, Paul Debevec -- University of Southern California Institute for Creative Technologies -- Eurographics Symposium on Rendering 2009.


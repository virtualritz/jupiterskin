Jupiter Skin
=============
Jupiter Skin is a project container for several skin shaders developed by `/*jupiter jazz*/`.

At the moment this project features the following shaders:

* Jupiter SGI Skin

Jupiter SGI Skin
----------------

Jupiter Spherical Gradient Illumination Skin is a
`RenderMan shading language <http://en.wikipedia.org/wiki/Shading_language#RenderMan_Shading_Language>`_ shader,
tested and used with `3Delight <http://www.3delight.com/>`_, that reads scanned RGB world space normal maps for 3 different wavelengths (red, green, blue), and a fourth normal map to be used in the specular term.
 
This technique, presented at: 

http://gl.ict.usc.edu/Research/FaceScanning/

allows the user to re-create the appear of the subtle *subsurface effects* of skin, together with *self-shadowing* and *inter-reflections*.

Once the texture are acquired:

* The three normal maps for the Lambertian diffuse term contain increasing detail as the light wavelength decreases.
* The normal map for the blue wavelength contains more detail than the normal map for the green wavelength, which in turn contains more detail than the red wavelength map.
* The specular normal map is used to drive the specular contribution (and the reflections), with the specular term being comprised of two specular lobes based on the Torrance-Sparrow model.

The user has the possibility of selecting between two microfacet distribution functions used:

* the *Beckmann microfacet distribution*, as suggested in the publication by Robert L. Cook and Kenneth E. Torrance, "A reflectance model for computer graphics"
* a *Gaussian distribution*, which is cheaper to compute and produces slightly softer results.

The weight of each of the specular lobe's contribution can also be controlled directly by the user, or it can be made automatically via the extracted data in the specular intensity image.

Notice also that this shader provides optionally a full two pass (point cloud based)subsurface term as well for greater flexibility.

This work has been sponsored by *Srinivas M. Mohan* (Indian Artists Computer Graphics Pvt. Ltd.) for the movie [http://en.wikipedia.org/wiki/Endhiran *Endhiran*], and it is based on:

* `"Rapid Acquisition of Specular and Diffuse Normal Maps from Polarized Spherical Gradient Illumination" <http://gl.ict.usc.edu/Research/FaceScanning/>`_ by Wan Chun Ma, Tim Hawkins, Pieter Peers, Charles-Felix Chabert, Malte Weiss, Paul Debevec -- University of Southern California Institute for Creative Technologies -- Eurographics Symposium on Rendering 2007.
* `"Estimating Specular Roughness and Anisotropy from Second Order Spherical Gradient Illumination" <http://gl.ict.usc.edu/Research/SpecularRoughness/>`_ by Abhijeet Ghosh, Tongbo Chen, Pieters Peers, Cyrus A. Wilson, Paul Debevec -- University of Southern California Institute for Creative Technologies -- Eurographics Symposium on Rendering 2009.


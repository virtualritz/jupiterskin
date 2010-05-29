Maya 2008 ascii file of a deformation test sequence.
The mesh deformation is somewhat extreme, but it illustrates well how the technique works with a single set of static data.
Paths in the assigned shaders obviously need to be properly set, the data used in this scene was the sample_face data that a user can retrieve from:
http://gl.ict.usc.edu/Research/FaceScanning/
To use a actual subsurface() pass, instead of relying solely on the RGBS technique, the user needs to pre-bake a point cloud file.


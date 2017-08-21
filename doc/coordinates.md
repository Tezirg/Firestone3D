# Coordinates system

## Handedness

Currently, F3D uses a right-handed system with counter clockwise faces ordering. There is no way to change the coordinate system used by F3D, such option will be considered for future releases. 

## Spaces definitions

| F3D Name    | Other names                           | Coordinates type  | Description                             |
|:------------|:--------------------------------------|:------------------|-----------------------------------------|
| Local       | Object space, Modeling space          | Carthesian 3D     | The local space is the coordinate system in which we define an object primitives. In local space, we do not take in consideration scale, orientation or translation. Hence, each object local space is independent from the other objects local space. |
| World       | Global space                          | Carthesian 3D     | In world space each objects are placed relative to each other using translation, rotation and scale operations. This result in a global space containing all the scene elements. |
| Camera      | View space                            | Carthesian 3D	  | When representing the camera in world space, it is set to an arbitrary position and orientation. In a view system, the camera is placed at the origin of the scene (0,0,0) and is oriented on the positive Z axis. Moving from world space to camera space requires all the objects to be transformed (camera transformations) so that the resulting view is unchanged. |
| Clip		  | Projective space 					  | Homogeneous       | Switching to homogeneous coordinate system allows for representation of perspective projection using a matrix. F3D enters clip space after perspective projection, face culling and primitive clipping (Corresponds to the vertex processing output). 
| Screen      |                  			          | Normalized device | Screen space coordinates are obtained after perspective division. Where a vertex clip coordinates is defined as (vx, vy, vz, vw) and in screen space defined as (vx/vw, vy/vw, vz/vw)
| Framebuffer | Viewport space, window space          | Carthesian 2D     | Framebuffer space represent a flat 2D space, mesured in pixels. Framebuffer space is reached by F3D after transforming the Screen coordinates with the viewport operations and running the rasterization algorithm on the primitives.

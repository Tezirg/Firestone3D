#ifndef _F3D_VERSION_H
#define _F3D_VERSION_H

const static int F3D_VERSION_MAJOR = 0;
const static int F3D_VERSION_MINOR = 1;
const static int F3D_VERSION_PATCH = 0;
const static int F3D_VERSION_TWEAK = 0;

#define F3D_VERSION_STRING "0.1.0.0"
#define F3D_MAKE_VERSION(major, minor, patch, tweak) \
		(((major) << 24) | ((minor) << 16) | ((patch) << 8) | (tweak))
#define F3D_VERSION F3D_MAKE_VERSION(F3D_VERSION_MAJOR, \
									 F3D_VERSION_MINOR, \
									 F3D_VERSION_PATCH, \
									 F3D_VERSION_TWEAK)
#define F3D_NAME "Firestone3D"

#endif

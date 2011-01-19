#ifndef THESKYATNIGHT_RESOURCES_H
#define THESKYATNIGHT_RESOURCES_H

// This seems to ignore the additional include folder setting. Yay for 
// relative paths.
#include "../cinder/include/cinder/CinderResources.h"

#define RES_STAR_TEXTURE0	CINDER_RESOURCE(resources/, star0.png, 128, GLSL)
#define RES_CLOUD_TEXTURE0	CINDER_RESOURCE(resources/, cloud0.png, 129, GLSL)

#endif // THESKYATNIGHT_RESOURCES_H

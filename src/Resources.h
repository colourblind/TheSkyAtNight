#ifndef THESKYATNIGHT_RESOURCES_H
#define THESKYATNIGHT_RESOURCES_H

// This seems to ignore the additional include folder setting. Yay for 
// relative paths.
#include "../cinder/include/cinder/CinderResources.h"

#define RES_STAR_TEXTURE0	CINDER_RESOURCE(resources/, star0.png, 128, GLSL)
#define RES_STAR_TEXTURE1	CINDER_RESOURCE(resources/, star1.png, 129, GLSL)
#define RES_STAR_TEXTURE2   CINDER_RESOURCE(resources/, star2.png, 130, GLSL)
#define RES_CLOUD_TEXTURE0	CINDER_RESOURCE(resources/, cloud0.png, 131, GLSL)

#endif // THESKYATNIGHT_RESOURCES_H

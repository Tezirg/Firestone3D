#pragma once

#ifndef _F3D_VALIDATION_H
#define _F3D_VALIDATION_H

#include <stdlib.h>

#ifdef _WIN32
#define ERR_EXIT(err_msg)														\
{																				\
	MessageBox(NULL, err_msg, "F3D failure", MB_OK);							\
	exit(1);																	\
} 
#elif defined __ANDROID__
#include <android/log.h>
#define ERR_EXIT(err_msg)														\
{																				\
    ((void)__android_log_print(ANDROID_LOG_INFO, "F3D", err_msg));				\
    exit(1);																	\
}
#else
#define ERR_EXIT(err_msg)														\
{																				\
        printf(err_msg);														\
        fflush(stdout);															\
        exit(1);																\
}
#endif

namespace f3d {
	namespace utils {
	}
}
#endif
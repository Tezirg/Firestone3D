#pragma once

#ifndef _F3D_VALIDATION_H
#define _F3D_VALIDATION_H

#include <stdlib.h>

#ifdef _WIN32

#include <windows.h>

#define F3D_ERROR(err_msg)														\
{																				\
	MessageBox(NULL, err_msg, "F3D Error", MB_OK);								\
}

#define F3D_FATAL_ERROR(err_msg)												\
{																				\
	MessageBox(NULL, err_msg, "F3D Fatal error", MB_OK);						\
	exit(EXIT_FAILURE);															\
} 




#elif defined __ANDROID__

#include <android/log.h>

#define F3D_FATAL_ERROR(err_msg)												\
{																				\
    ((void)__android_log_print(ANDROID_LOG_INFO, "F3D Fatal error", err_msg));	\
	exit(EXIT_FAILURE);															\
}

#define F3D_ERROR(err_msg)														\
{																				\
    ((void)__android_log_print(ANDROID_LOG_INFO, "F3D Error", err_msg));		\
}

#else

#define F3D_FATAL_ERROR(err_msg)												\
{																				\
    printf("F3D Fatal error: %s", err_msg);										\
    fflush(stdout);																\
	exit(EXIT_FAILURE);															\
}

#define F3D_ERROR(err_msg)														\
{																				\
    printf("F3D Error: %s", err_msg);											\
    fflush(stdout);																\
}

#endif

#define F3D_ASSERT(test, err_msg)												\
{																				\
	if (test == false) {														\
		std::string		msg("F3D assertion failed: ");							\
		msg.append(err_msg);													\
		F3D_FATAL_ERROR(msg.c_str());											\
	}																			\
}

#endif
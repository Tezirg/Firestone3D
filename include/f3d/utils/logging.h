#pragma once

#ifndef _F3D_VALIDATION_H
#define _F3D_VALIDATION_H

#include <stdlib.h>

#ifdef _WIN32

#include <windows.h>

#define F3D_FATAL_ERROR(err_msg)												\
{																				\
	MessageBox(NULL, err_msg, "F3D failure", MB_OK);							\
	exit(EXIT_FAILURE);															\
} 

#elif defined __ANDROID__

#include <android/log.h>

#define F3D_FATAL_ERROR(err_msg)												\
{																				\
    ((void)__android_log_print(ANDROID_LOG_INFO, "F3D", err_msg));				\
	exit(EXIT_FAILURE);															\
}

#else

#define F3D_FATAL_ERROR(err_msg)												\
{																				\
    printf(err_msg);															\
    fflush(stdout);																\
	exit(EXIT_FAILURE);															\
}
#endif

#endif
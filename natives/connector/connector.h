#pragma once

#include <jni.h>

#ifdef __GNUC__
#include <stdint.h>
#define jintptr_t intptr_t
#define CONNECTOR_EXPORT __attribute__ ((visibility("default"))) JNIEXPORT
#else
#define CONNECTOR_EXPORT JNIEXPORT
#endif

#ifdef MSC_VER
#define jintptr_t __int3264
#define CONNECTOR_IMPORT __declspec(dllimport)
#else
#define CONNECTOR_IMPORT
#endif

// Created by Przemys³aw Wiewióra 2020

// This should be included once in each .cpp file at the top before other includes.
// In case of subproject including this files it should be under ProjectPCH.h but still above everything else than that.

// CoreEngine.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

// Add headers that you want to pre-compile here 

#ifdef LIBRARY_EXPORTS
//#    define LIBRARY_API __declspec(dllexport)
#else
//#    define LIBRARY_API __declspec(dllimport)
#endif

#include "Includes/FullCppIncludes.h"
#include "Includes/FullThirdParty.h"
#include "Includes/FullEngineBase.h"

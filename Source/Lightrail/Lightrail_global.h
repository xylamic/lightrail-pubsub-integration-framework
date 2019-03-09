/*!
@file	Lightrail_global.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef LIGHTRAIL_GLOBAL_H
#define LIGHTRAIL_GLOBAL_H

#include <Xylasoft.h>

#ifdef WIN32
#	if defined(LIGHTRAIL_LIB)
#		define LIGHTRAILSHARED_EXPORT __declspec(dllexport)
#	else
#		define LIGHTRAILSHARED_EXPORT __declspec(dllimport)
#	endif
#else
#	define LIGHTRAILSHARED_EXPORT
#endif

#define LIGHTRAILSHARED_EXPORTTEST 
//#define LIGHTRAILSHARED_EXPORTTEST LIGHTRAILSHARED_EXPORT

#endif // LIGHTRAIL_GLOBAL_H

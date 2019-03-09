/*!
@file	Junction.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef JUNCTION_H
#define JUNCTION_H

#include "Lightrail_global.h"
#include <XThread.h>

namespace Xylasoft
{
	class Junction : protected XThread
	{
	public:
		Junction(const wchar_t* filepath);
		virtual ~Junction();

		void StartJunction();

		void StopJunction();

	protected:
		virtual void Execution() override;

	private:
		std::wstring m_filepath;
	};
}

#endif
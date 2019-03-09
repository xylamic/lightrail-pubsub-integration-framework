/*!
@file	SimpleListLightrail.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef SIMPLELISTLIGHTRAIL_H
#define SIMPLELISTLIGHTRAIL_H

#include "Lightrail_global.h"
#include "SimpleListContainer.h"

namespace Xylasoft
{
	template < typename T >
	class SimpleListLightrail : public SimpleListContainer<T>
	{
	public:
		virtual ~SimpleListLightrail() { }

		virtual void SetElements(const std::vector<T>& elementList)
		{
			delete this->m_data;

			this->m_size = static_cast<long>(elementList.size());
			this->m_data = new T[elementList.size()];

			int index = 0;
			for (auto listit = elementList.begin(); listit != elementList.end(); listit++)
			{
				this->m_data[index] = *listit;
				index++;
			}
		}
	};
}

#endif
/*!
@file	SimpleListContainer.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef SIMPLELISTCONTAINER_H
#define SIMPLELISTCONTAINER_H

#include "Lightrail_global.h"

namespace Xylasoft
{
	/*!
	This defines a generic container for listing objects across binary
	boundaries.
	*/
	template < typename T >
	class LIGHTRAILSHARED_EXPORT SimpleListContainer
	{
	public:
		/*!
		Construct an empty container.
		*/
		SimpleListContainer()
		{
			this->m_size = 0;
			this->m_data = nullptr;
		}

		/*!
		Destroy the container.
		*/
		virtual ~SimpleListContainer()
		{
			delete this->m_data;
		}

		/*!
		Get the size of the container.
		*/
		virtual int Size()
		{
			return this->m_size;
		}

		/*!
		Get the list of elements in the container.
		*/
		virtual T* Elements()
		{
			return this->m_data;
		}

	protected:
		int m_size;
		T* m_data;

	private:
		SimpleListContainer(const SimpleListContainer& other);
		SimpleListContainer& operator=(const SimpleListContainer& other);
	};
}

#endif
/*!
@file	XEventCallback.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XEVENTCALLBACK_H
#define XEVENTCALLBACK_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*!
	The interface for a callback from an event.
	@param Parameter The type of parameter that will be sent with the event.
	*/
	template < typename Parameter >
	class IXEventCallback
	{
	public:
		/*!
		Execute the callback.
		@param[in] parameter The parameter to pass to the event.
		*/
		virtual void Execute(Parameter parameter) = 0;
	};

	/*!
	The implementation for an event specific to the callback class.
	@param Class The type of class that will be called. (Your class type)
	@param Parameter The parameter that will be passed for the event. This
	must match the Parameter of the IXEventCallback.
	*/
	template < class Class, typename Parameter >
	class XEventCallback : public IXEventCallback < Parameter >
	{
	public:
		/*!
		Type definition for a method within the specified class.
		*/
		typedef void(Class::*Method)(Parameter);

		/*!
		Constructor for an event callback.
		@param[in] classInstance The instance of the class that will
		be called.
		@param[in] method The method of the class that will be called.
		*/
		XEventCallback(Class* classInstance, Method method)
		{
			this->m_classInstance = classInstance;
			this->m_method = method;
		}

		/*!
		Execute the callback.
		@param[in] parameter The parameter to pass for the callback.
		*/
		virtual void Execute(Parameter parameter)
		{
			return (this->m_classInstance->*this->m_method)(parameter);
		}

	private:
		Class* m_classInstance;
		Method m_method;
	};

}
#endif // XEventCallback_H

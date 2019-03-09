/*!
@file	XSingleEventCallback.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XSINGLEEVENTCALLBACK_H
#define XSINGLEEVENTCALLBACK_H

#include "Xylasoft.h"
#include "XEventCallback.h"
#include "XException.h"

namespace Xylasoft
{
	/*!
	This defines a mechanism for single parameter events for single receivers.
	@param Parameter The parameters to be used in callback.
	*/
	template < typename Parameter >
	class XSingleEventCallback : public IXEventCallback<Parameter>
	{
	public:
		/*!
		Constructor. Defines an empty callback.
		*/
		XSingleEventCallback()
			: m_callback(nullptr)
		{
		}

		/*!
		Destructor. Deletes the callback if necessary.
		*/
		virtual ~XSingleEventCallback()
		{
			delete this->m_callback;
		}

		/*!
		Set the specified event callback as the callback for this event.
		*/
		void SetCallback(IXEventCallback<Parameter>* callback)
		{
			if (callback == nullptr)
			{
				throw ArgumentException(L"The callback cannot be nullptr.");
			}

			delete this->m_callback;
			this->m_callback = callback;
		}

		/*!
		Remove the callback, so no callback will execute.
		*/
		void RemoveCallback()
		{
			delete this->m_callback;
			this->m_callback = nullptr;
		}

		/*!
		Execute the callback.
		@param parameter The parameter to send to the listener.
		*/
		virtual void Execute(Parameter parameter)
		{
			if (this->m_callback != nullptr)
			{
				this->m_callback->Execute(parameter);
			}
		}

	private:
		IXEventCallback<Parameter>* m_callback;
	};
}

#endif
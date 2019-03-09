/*!
@file	XEvent.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XEVENT_H
#define XEVENT_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*!
	Event wait handle for synchronization of threads.
	*/
	class XEvent
	{
	public:
		/*!
		Construct the event handle.
		*/
		XEvent();

		/*!
		Destroy the event handle.
		*/
		~XEvent();

		/*!
		Signal anything waiting on this event. Stays signaled until reset.
		*/
		void Signal();

		/*!
		Reset the event so it is no longer signaled.
		*/
		void Reset();

		/*!
		Wait for the event handle for the specified time.
		@param[in] timeout The time to wait for the event.
		@returns True if signaled, false otherwise.
		*/
		bool Wait (int timeout = 30000);

	private:
		HANDLE m_handle;
	};
}

#endif
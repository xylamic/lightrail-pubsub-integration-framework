/*!
@file	XEvent.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XEvent.h"

using namespace Xylasoft;

XEvent::XEvent ()
{
	// start in non-signaled state (red light)
	// auto reset after every Wait
	m_handle = ::CreateEvent(0, TRUE, FALSE, 0);
}

XEvent::~XEvent ()
{
	::CloseHandle (this->m_handle);
}

void XEvent::Signal ()
{
	::SetEvent (this->m_handle);
}

void XEvent::Reset()
{
	::ResetEvent(this->m_handle);
}

bool XEvent::Wait (int timeout)
{
	// Wait until event is in signaled (green) state
	if (::WaitForSingleObject (this->m_handle, timeout) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
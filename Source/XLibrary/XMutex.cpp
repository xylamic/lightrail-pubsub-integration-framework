/*!
@file	XMutex.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XMutex.h"

using namespace Xylasoft;

XMutex::XMutex()
{
	::InitializeCriticalSection(&this->m_criticalSection);
}

XMutex::~XMutex()
{
	::DeleteCriticalSection(&this->m_criticalSection);
}

void XMutex::Lock()
{
	::EnterCriticalSection(&this->m_criticalSection);
}

void XMutex::Unlock()
{
	::LeaveCriticalSection(&this->m_criticalSection);
}

XMutexLocker::XMutexLocker(XMutex& mutex)
	: m_mutex(mutex)
{
	this->m_mutex.Lock();
	this->m_locked = true;
}

XMutexLocker::~XMutexLocker()
{
	if (this->m_locked)
	{
		this->m_mutex.Unlock();
	}
}

void XMutexLocker::Relock()
{
	if (this->m_locked)
	{
		return;
	}

	this->m_mutex.Lock();
	this->m_locked = true;
}

void XMutexLocker::Unlock()
{
	if (!this->m_locked)
	{
		return;
	}

	this->m_mutex.Unlock();
	this->m_locked = false;
}
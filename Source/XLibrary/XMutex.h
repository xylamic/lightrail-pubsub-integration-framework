/*!
@file	XMutex.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XMUTEX_H
#define XMUTEX_H

#include "Xylasoft.h"

namespace Xylasoft
{
	/*!
	This is a cross platform mutex for safeguarding critical sections.
	*/
	class XMutex
	{
	public:
		/*!
		Construct a mutex.
		*/
		XMutex();

		/*!
		Destroy the mutex.
		*/
		~XMutex();

		/*!
		Lock the mutex. Wait for unlock if currently locked by another thread.
		*/
		void Lock();

		/*!
		Unlock the mutex.
		*/
		void Unlock();

	private:
		CRITICAL_SECTION m_criticalSection;
	};

	/*!
	Provides a stack-based container for a Mutex to allow ease of lock/unlock and protect
	against falling out of execution and leaving a mutex locked.
	*/
	class XMutexLocker
	{
	public:
		/*!
		Create the mutex locker with an unlocked mutex. Locks the mutex.
		@param[in] mutex The mutex to contain.
		*/
		XMutexLocker(XMutex& mutex);

		/*!
		Destroy the mutex. Unlocks if locked.
		*/
		~XMutexLocker();

		/*!
		Relock the mutex after manually unlocking.
		*/
		void Relock();

		/*!
		Unlock the mutex. Does not need to be relocked before destroy.
		*/
		void Unlock();

	private:
		XMutex& m_mutex;
		volatile bool m_locked;
	};
}

#endif
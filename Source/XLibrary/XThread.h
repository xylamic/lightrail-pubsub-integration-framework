/*!
@file	XThread.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XTHREAD_H
#define XTHREAD_H

#include "Xylasoft.h"

namespace Xylasoft {
	/*!
	The case class for threading operations.
	*/
	class XThread
	{
	public:
		/*!
		Construct a new thread.
		*/
		XThread();

		/*!
		Deconstruct the thread.
		*/
		virtual ~XThread();

		/*!
		Mark the thread to die as soon as possible.
		*/
		void Kill();

		/*!
		Start the thread.
		*/
		void Start();

		/*!
		Get whether the thread is running.
		*/
		bool IsRunning();

		/*!
		Wait the specified amount of time for the thread to end in milliseconds.
		@param[in] timeout The time to wait before timeout.
		@returns True if the thread ended, false otherwise.
		*/
		bool Wait(int timeout = 30000);

	protected:
		/*!
		Method to execution before primary execution within the thread.
		*/
		virtual void PreExecution();

		/*!
		Method for execution in the primary logic within the thread. This
		must be implemented by the base class to be executed when the thread
		is started.
		*/
		virtual void Execution() = 0;

		/*!
		Method for execution after primary execution within the thread.
		*/
		virtual void PostExecution();

		/*!
		Tell the thread to sleep for the specified period of time.
		@param[in] timeout The time to wait in sleep in milliseconds.
		*/
		void Sleep(int timeout = 0);

		/*!
		Check if the thread is marked to die.
		@returns True if the thread should die as soon as possible.
		*/
		bool ShouldDie();

	private:
		volatile bool m_isDying;

		volatile bool m_isRunning;

		// this defines a private, platform-specific section that should never
		// be apparent to other classes or derived classes
	private:
		/*!
		This is the windows thread entry point.
		@param[in] pArg The thread arguments.
		*/
		static DWORD WINAPI ThreadEntry(void* pArg);

		HANDLE m_threadHandle;
		DWORD m_threadId;
	};

}
#endif
/*!
@file	XThread.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "XThread.h"

using namespace Xylasoft;

XThread::XThread()
{
	m_threadHandle = ::CreateThread (
						0, // Security attributes
						0, // Stack size
						ThreadEntry,
						this,
						CREATE_SUSPENDED,
						&this->m_threadId);

	this->m_isRunning = false;
	this->m_isDying = false;
}

XThread::~XThread()
{
	if (this->IsRunning())
	{
		this->Kill();
		assert(this->Wait());
	}

	::CloseHandle(this->m_threadHandle);
}

void XThread::Kill()
{
	this->m_isDying = true;
}

void XThread::Start()
{
	::ResumeThread(this->m_threadHandle);
}

bool XThread::IsRunning()
{
	return this->m_isRunning;
}

bool XThread::Wait(int timeout)
{
	if (::WaitForSingleObject(this->m_threadHandle, timeout) == WAIT_OBJECT_0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void XThread::PreExecution()
{
}

void XThread::PostExecution()
{
}

void XThread::Sleep(int timeout)
{
	::Sleep(timeout);
}

bool XThread::ShouldDie()
{
	return this->m_isDying;
}

DWORD WINAPI XThread::ThreadEntry(void* pArg)
{
	try
	{
		XThread* pxThread = reinterpret_cast<XThread*>(pArg);

		pxThread->m_isDying = false;
		pxThread->m_isRunning = true;

		pxThread->PreExecution();
		pxThread->Execution();
		pxThread->PostExecution();

		pxThread->m_isRunning = false;

		return 0;
	}
#if _DEBUG
	catch (std::exception& ex)
	{
		std::cout << "CRITICAL_THREAD_ERROR: " << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "CRITICAL_THREAD_ERROR: Ellipical error." << std::endl;
	}
#else
	catch (...)
	{
	}
#endif

	return -1;
}
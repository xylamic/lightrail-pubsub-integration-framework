/*!
@file	XQueue.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef XQUEUE_H
#define XQUEUE_H

#include "Xylasoft.h"
#include "XMutex.h"
#include <queue>

namespace Xylasoft
{
	/*!
	This is a standard queue container that is thread-safe.
	@param T The type for the queue to contain.
	*/
	template < typename T >
	class XQueue
	{
	public:
		template < typename T >
		struct XQueueItem
		{
			long Size;
			T Item;
		};

	public:
		/*!
		Construct an empty queue.
		*/
		XQueue()
		{
			this->m_memorySize = 0;
		}

		/*!
		Clear the queue. Removes all items.
		*/
		void Clear()
		{
			XMutexLocker locker(this->m_queueLock);

			this->m_queue.clear();
			this->m_memorySize = 0;
		}

		/*!
		Enqueue a value.
		@param[in] t The value to queue.
		*/
		void Enqueue(T t, long memorySize = -1)
		{
			XMutexLocker locker(this->m_queueLock);

			// create the item
			XQueueItem<T> item;
			
			// set the memory size
			if (memorySize < 0)
			{
				item.Size = sizeof(T);
			}
			else
			{
				item.Size = memorySize;
			}
			this->m_memorySize += item.Size;

			// set the item
			item.Item = t;

			this->m_queue.push(item);
		}

		/*!
		Dequeue a value.
		*/
		void Dequeue()
		{
			XMutexLocker locker(this->m_queueLock);

			// get the item
			XQueueItem<T> item = this->m_queue.front();

			// remove the memory size
			this->m_memorySize -= item.Size;

			this->m_queue.pop();
		}

		/*!
		Peek at the front of the queue.
		@returns The first item on the queue.
		*/
		T Peek()
		{
			XMutexLocker locker(this->m_queueLock);

			XQueueItem<T> item = this->m_queue.front();
			return item.Item;
		}

		/*!
		Peek and dequeue. Returns the front of the queue and dequeues it from the queue.
		@returns The front of the queue.
		*/
		T PeekDequeue()
		{
			XMutexLocker locker(this->m_queueLock);

			T t = this->Peek();
			this->Dequeue();
			
			return t;
		}

		/*!
		Get the size of the queue.
		@returns The number of items in the queue.
		*/
		long Size()
		{
			XMutexLocker locker(this->m_queueLock);

			return static_cast<long>(this->m_queue.size());
		}

		/*!
		Get the amount of memory used by this queue.
		@returns The size of memory in bytes.
		*/
		long MemorySize()
		{
			XMutexLocker locker(this->m_queueLock);

			return this->m_memorySize;
		}

	private:
		std::queue<XQueueItem<T>> m_queue;
		long m_memorySize;
		XMutex m_queueLock;
	};
}

#endif
#ifndef JUNCTION_H
#define JUNCTION_H

#include"LightrailJunction_global.h"
#include <Lightrail.h>
#include <XThread.h>

namespace Xylasoft
{
	class Junction : protected XThread
	{
	public:
		Junction()
		{
			// load configuraton

			// create lightrail instance
		}

		~Junction()
		{
			// delete lightrail instance
		}

		void StartJunction()
		{
		}

		void StopJunction()
		{
		}

	protected:
		virtual void PreExecution() override
		{
		}

		virtual void Execution() override
		{
		}

	private:
		ILightrail* m_localClient;
	};
}

#endif
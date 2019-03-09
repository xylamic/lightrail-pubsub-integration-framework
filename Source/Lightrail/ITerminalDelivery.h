/*!
@file	ITerminalDelivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef ITERMINALDELIVERY_H
#define ITERMINALDELIVERY_H

#include "Lightrail_global.h"
#include "IDelivery.h"
#include "IDetailedDelivery.h"
#include "ITerminal.h"

namespace Xylasoft
{
	class ITerminalDelivery
	{
	public:
		/*!
		Destructor. Destroys the containing items.
		*/
		virtual ~ITerminalDelivery() { }

		/*!
		Get the delivery.
		*/
		virtual const IDetailedDelivery* DetailedDelivery() const = 0;

		/*!
		Get the terminal this delivery was received on.
		@param[in] The index of the terminal.
		*/
		virtual const ITerminal* ReceiptTerminal(int index) const = 0;

		/*!
		Get the number of terminals this delivery was received at. This is a zero-based
		index to use to retrieve an individual terminal with ReceiptTerminal(int).
		*/
		virtual long NumberOfReceiptTerminals() const = 0;
	};

	class ITerminalReturnDelivery : public ITerminalDelivery
	{
	public:
		/*!
		Destructor. Destroy the containing items.
		*/
		virtual ~ITerminalReturnDelivery() { }

		/*!
		Get the delivery to be returned to the sender.
		@returns The delivery to contain the response.
		*/
		virtual IDelivery* ReturnedDelivery() const = 0;
	};
}

#endif
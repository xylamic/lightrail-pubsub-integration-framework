/*!
@file	TerminalDelivery.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#ifndef TERMINALDELIVERY_H
#define TERMINALDELIVERY_H

#include "Lightrail_global.h"
#include "IDelivery.h"
#include "IDetailedDelivery.h"
#include "ITerminal.h"
#include "ITerminalDelivery.h"
#include "Terminal.h"

namespace Xylasoft
{
	/*!
	This contains a message to be delivered to a terminal.
	*/
	class TerminalDelivery : virtual public ITerminalDelivery
	{
	public:
		/*!
		Construct a terminal delivery.
		@param[in] delivery The message to deliver.
		@param[in] receiptTerminals The terminals this message was received on.
		*/
		TerminalDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals);

		/*!
		Destructor. Destroys the containing items.
		*/
		virtual ~TerminalDelivery();

		/*!
		Get the delivery.
		*/
		virtual const IDetailedDelivery* DetailedDelivery() const override;

		/*!
		Get the terminal this delivery was received on.
		@param[in] The index of the terminal.
		*/
		virtual const ITerminal* ReceiptTerminal(int index) const override;

		/*!
		Get the number of terminals this delivery was received at. This is a zero-based
		index to use to retrieve an individual terminal with ReceiptTerminal(int).
		*/
		virtual long NumberOfReceiptTerminals() const override;

	private:
		const IDetailedDelivery* m_delivery;
		std::vector<Terminal> m_terminals;
	};

	class TerminalResponseDelivery : public TerminalDelivery
	{
	public:
		/*!
		Construct a terminal delivery for a response to a request.
		@param[in] delivery The message to deliver.
		@param[in] receiptTerminals The terminals this message was received on.
		*/
		TerminalResponseDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals);
	};

	class TerminalSubscriptionDelivery : public TerminalDelivery
	{
	public:
		/*!
		Construct a terminal delivery for a subscription.
		@param[in] delivery The message to deliver.
		@param[in] receiptTerminals The terminals this message was received on.
		*/
		TerminalSubscriptionDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals);
	};

	/*!
	This contains a request made to a terminal and contains the response to be delivered.
	*/
	class TerminalRequestDelivery : public TerminalDelivery, public ITerminalReturnDelivery
	{
	public:
		/*!
		Construct a terminal response delivery.
		@param[in] delivery The message to deliver.
		@param[in] receiptTerminals The terminals this message was received on.
		*/
		TerminalRequestDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals);

		/*!
		Get the delivery.
		*/
		virtual const IDetailedDelivery* DetailedDelivery() const override;

		/*!
		Get the terminal this delivery was received on.
		@param[in] The index of the terminal.
		*/
		virtual const ITerminal* ReceiptTerminal(int index) const override;

		/*!
		Get the number of terminals this delivery was received at. This is a zero-based
		index to use to retrieve an individual terminal with ReceiptTerminal(int).
		*/
		virtual long NumberOfReceiptTerminals() const override;

		/*!
		Get the delivery to be returned to the sender.
		@returns The delivery to contain the response.
		*/
		virtual IDelivery* ReturnedDelivery() const override;

	private:
		IDelivery* m_delivery;
	};
}

#endif
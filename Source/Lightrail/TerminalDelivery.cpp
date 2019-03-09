/*!
@file	TerminalDelivery.cpp
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "TerminalDelivery.h"
#include "Delivery.h"

using namespace Xylasoft;

TerminalDelivery::TerminalDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals)
	: m_delivery(delivery), m_terminals(receiptTerminals)
{ }

TerminalDelivery::~TerminalDelivery()
{
	delete this->m_delivery;
}

const IDetailedDelivery* TerminalDelivery::DetailedDelivery() const
{
	return this->m_delivery;
}

const ITerminal* TerminalDelivery::ReceiptTerminal(int index) const
{
	return &this->m_terminals[index];
}

long TerminalDelivery::NumberOfReceiptTerminals() const
{
	return static_cast<long>(this->m_terminals.size());
}

TerminalResponseDelivery::TerminalResponseDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals)
	: TerminalDelivery(delivery, receiptTerminals)
{ }

TerminalSubscriptionDelivery::TerminalSubscriptionDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals)
	: TerminalDelivery(delivery, receiptTerminals)
{ }

TerminalRequestDelivery::TerminalRequestDelivery(const IDetailedDelivery* delivery, std::vector<Terminal>& receiptTerminals)
	: TerminalDelivery(delivery, receiptTerminals), m_delivery(new Delivery())
{ }

const IDetailedDelivery* TerminalRequestDelivery::DetailedDelivery() const
{
	return TerminalDelivery::DetailedDelivery();
}

const ITerminal* TerminalRequestDelivery::ReceiptTerminal(int index) const
{
	return TerminalDelivery::ReceiptTerminal(index);
}

long TerminalRequestDelivery::NumberOfReceiptTerminals() const
{
	return TerminalDelivery::NumberOfReceiptTerminals();
}

IDelivery* TerminalRequestDelivery::ReturnedDelivery() const
{
	return this->m_delivery;
}
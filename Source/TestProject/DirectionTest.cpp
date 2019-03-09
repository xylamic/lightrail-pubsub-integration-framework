/*!
@file	XHostAddress.h
@author	Adam Jordan

This file is part of the Xylasoft Lightrail product.

Copyright (c) 2011 Adam Jordan, Xylasoft, All Rights Reserved
http://www.xylasoft.com

Licensed for use under the MIT License
See accompanying LICENSE file for more information
*/

#include "Tests.h"

#ifndef EXCLUDETESTLIBS

bool ValidTopic(const wchar_t* topic)
{
	try
	{
		Terminal t(L"name", L"desc", topic);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void ValidationTest()
{
	assert(!ValidTopic(L"jflsf>>fjs]"));
	assert(!ValidTopic(L"[jflsf>>fjs>]"));
	assert(!ValidTopic(L"[jflsf>>fjs"));
	assert(!ValidTopic(L"[>>>>>]"));
	assert(!ValidTopic(L"[>]"));
	assert(!ValidTopic(L"[]"));
	assert(!ValidTopic(L"[>"));

	assert(ValidTopic(L"[>jflsf>>fjs]"));
	assert(ValidTopic(L"[a]"));
	assert(ValidTopic(L"[a>"));
	assert(ValidTopic(L"[abc>flsf>>>a]"));
	assert(ValidTopic(L"[ajfls>"));
	assert(ValidTopic(L"[a>>>>>"));
	assert(ValidTopic(L"[a>>>>b]"));
}

void TerminalCompare()
{
	assert(Terminal(L"", L"", L"[a]") == Terminal(L"1", L"2", L"[a]"));
	assert(Terminal(L"", L"", L"[ae>b]") == Terminal(L"1", L"2", L"[ae>b]"));
	assert(Terminal(L"", L"", L"[ad>b]") == Terminal(L"1", L"2", L"[ad>"));
	assert(Terminal(L"", L"", L"[ad>bes>") == Terminal(L"1", L"2", L"[ad>"));
	assert(Terminal(L"", L"", L"[ad>>>>") == Terminal(L"1", L"2", L"[ad>"));
	assert(Terminal(L"", L"", L"[>ab>ab]") == Terminal(L"1", L"2", L"[ad>"));
	assert(Terminal(L"", L"", L"[ad>>>>") == Terminal(L"1", L"2", L"[ad>"));
	assert(Terminal(L"", L"", L"[ad>gis>fos>ofis>fisl]") == Terminal(L"1", L"2", L"[ad>"));

	assert(Terminal(L"", L"", L"[ad>gis>fos>ofis>fisl]") != Terminal(L"1", L"2", L"[ad]"));
	assert(Terminal(L"", L"", L"[ad>gis>fos>ofis>fisl]") != Terminal(L"1", L"2", L"[ad>gis>fos>ofis]"));
	assert(Terminal(L"", L"", L"[ade]") != Terminal(L"1", L"2", L"[adbs]"));
	assert(Terminal(L"", L"", L"[ade>abc>abc>abc]") != Terminal(L"1", L"2", L"[ade>abd>abc>"));
	assert(Terminal(L"", L"", L"[ade]") != Terminal(L"1", L"2", L"[adbs>"));
}

void TerminalTest()
{
	cout << "Starting terminal tests..." << endl;

	::ValidationTest();
	::TerminalCompare();

	cout << "Completed terminal tests." << endl;
}

#else
void TerminalTest()
{
}
#endif
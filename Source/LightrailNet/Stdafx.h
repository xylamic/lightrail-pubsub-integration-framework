// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <XException.h>

#define MANAGEDTRYBEGIN try {
#define MANAGEDTRYEND	} \
	catch (Xylasoft::TimeoutException& ex) { throw gcnew System::TimeoutException(gcnew String(ex.what())); } \
	catch (Xylasoft::IOException& ex) { throw gcnew System::IO::IOException(gcnew String(ex.what())); } \
	catch (Xylasoft::NetworkException& ex) { throw gcnew System::IO::IOException(gcnew String(ex.what())); } \
	catch (Xylasoft::ArgumentException& ex) { throw gcnew System::ArgumentException(gcnew String(ex.what())); } \
	catch (Xylasoft::DataException& ex) { throw gcnew System::FormatException(gcnew String(ex.what())); } \
	catch (std::exception& ex) { throw gcnew Exception(gcnew String(ex.what())); } \
	catch (...) { throw gcnew Exception("Unknown failure."); }
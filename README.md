# lightrail-pubsub-integration-framework
Lightrail is a cross-platform publish/subscribe integration built directly upon sockets for use across nearly any operating system (or embedded device). 

Lightrail is a publish/subscribe framework targeted at high-performance applications that primarily reside on the same subset. This is useful for many-node systems, such as trading systems, or embedded IoT systems where many small devices many need to communicate efficiently.

Publish/subscribe applications communicate via "Terminals" that are registered to publish, subscribe, request, or respond to information. Those channels can be secured with a security key (symmetric) as needed.

Further details on what it is and how it works is available in the [Lightrail Wiki](https://github.com/xylamic/lightrail-pubsub-integration-framework/wiki)

# Code Base
The core Lightrail framework is built in C++ and has been verified on Windows and multiple Linux distributions to build/work properly. It also includes C# bindings for use and testing on Windows. The majority of the development takes place on Windows and then debugging on other platforms. The included Windows solution can be opened and built (newer version of Visual Studio) with no additional dependencies.

## Projects and Dependencies
Below lists the projects in to the codebase and the order in which they are to be referred/built.
### Core Framework (C++ Lightrail framework)
1. XTools: generic C++ utilities
2. XLibrary: tool library for use in cross-platform, low-level applications
3. Lightrail: core C++ library
4. Lightrail console: simple console application for using/testing the C++ framework
### C# Interop & Tools:
1. LightrailNet: interop library for Lightrail
2. LightrailNetTest: test classes for the framework
3. LightrailDiagnostics: WinForms applications for sending/receiving messages and monitoring network activity

# License
The code is provided under the MIT license.

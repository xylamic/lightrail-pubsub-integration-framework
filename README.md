# lightrail-pubsub-integration-framework
Lightrail is a cross-platform publish/subscribe integration built directly upon sockets for use across nearly any operating system (or embedded device). It's focused on high-performance, high-efficiency within the same subnet with ZERO configuration.

# Detailed Description
Lightrail is a publish/subscribe framework targeted at high-performance applications that primarily reside on the same subset. This is useful for many-node systems, such as trading systems, or embedded IoT systems where many small devices many need to communicate efficiently.

Publish/subscribe applications communicate via "Terminals" that are registered to publish, subscribe, request, or respond to information. Those channels can be secured with a security key (symmetric) as needed.

Details on how it is intended to be used and integrated is available in the wiki.

# Code Base
The core Lightrail framework is built in C++ and has been verified on Windows and multiple Linux distributions to build/work properly. It also includes C# bindings for use and testing on Windows. The majority of the development takes place on Windows and then debugging on other platforms. The included Windows solution can be opened and built (newer version of Visual Studio) with no additional dependencies.

# License
The code is provided under the MIT license.

# bRAWcap C++ Wrapper

This project contains a C++ wrapper for the bRAWcap C API.
bRAWcap is a high performance network driver for Windows which allows to send and receive raw Ethernet packets
from any 802.3 Ethernet adapter supported by Windows.

Its features and functionality is similar to those available with winpcap (based on libpcap).
But the API is not yet available with a libpcap interface - this may be added soon.

For now it is required to directly use the bRAWcap C API and for easier integration and object oriented programming
b-plus provides this C++ wrapper which is open source available to everybody and free to use.

You are allowed to also extend the C++ wrapper to your own needs - without guarantee.
If you find any improvement or issue in the C++ wrapper we would appreciate your feedback (e.g. via issue request).

## Motivation

Since winpcap is no longer developed anymore and is not designed for high bandwidth capturing (e.g. on multiple 10G ports)
b-plus has developed it´s own driver to provide a solution which fulfills the occurring data rates in todays
industry/automotive environments.

## Contents

1. The project contains the complete C++ wrapper which will be updated for every new version of bRAWcap (and it´s C API).
   Therefore it will always contain all available features which will be added to the C API itself.
   The C++ wrapper is located in the [`src\*`](https://github.com/bplus-group/bRAWcap-CPP-Wrapper/tree/main/src) subdirectory.

1. Because the C++ wrapper is based on (makes use of) the bRAWcap C API there is also a copy of the corresponding
   C API (which is part of the bRAWcap SDK) available in the repository.
   The bRAWcap C API is located at [`sdk\c\`](https://github.com/bplus-group/bRAWcap-CPP-Wrapper/tree/main/sdk/c).
   In that subdirectory there is everything required for building the bRAWcap C++ wrapper.
   If you want to build your application directly based on the C API there is also a full HTML documentation available
   at [`sdk\c\doc\html`](https://github.com/bplus-group/bRAWcap-CPP-Wrapper/tree/main/sdk/c/doc/html).
   The documentation is generated with doxygen and therefore also directly inlined in the C API header files.

## Building

The C++ wrapper code is based on C++17 features, therefore it is recommended to use a compiler
which supports at least C++17, otherwise you may get compiler errors due to the C++ wrapper.

We always check builds with MSVC 2017 compiler (part of Windows SDK 1809).

It is also necessary to add the path to the main header of the bRAWcap C API to the include paths.
In case of our repository structure here this would be `sdk\c\include\`.

## Using

Always include the [brawcap.hpp](https://github.com/bplus-group/bRAWcap-CPP-Wrapper/blob/main/src/brawcap.hpp)
header to get full access to all features.

It is recommended to always create one **BRAWcap** instance for each connection to a adapter/interface.
This instance provides access to all features such as sending and receiving raw Ethernet packets
to/from the specified adapter.

If instead only receiving or transmitting is required it is also possible to directly create a instance of
**BRAWcapReceive** or **BRAWcapTransmit**.

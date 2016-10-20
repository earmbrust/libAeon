## libAeon

libaeon is a cross platform socket library designed for fast and efficient
network communications.  

## HTTP Client Code Example
First, include the library header:
```cpp
#include <libaeon.h>
```

Then you'll need to create an instance of the net::CClientSocket class:
```cpp
sHostArg = argv[1];
if (sHostArg.find("http://") != std::string::npos) sHostArg = sHostArg.substr(7);
sPage = "/";
if (sHostArg.find("/") != std::string::npos)
{
    Page = sHostArg.substr(sHostArg.find("/"));
    sDomain = sHostArg.substr(0, sHostArg.find("/"));
} else {
    sDomain = sHostArg;
}
net::CClientSocket *sockClient = new net::CClientSocket(&sDomain, 80);
```
WIP

## Motivation
It was designed with the thought in mind that
network programming should not be as arbitrary and vague as it currently is.
libaeon adds the "net" namespace to c++ programs utilizing it, to make
the separation of class/library specific functions from already defined
functions as well as to follow conventions already in place for c++
programmers.  Installation is simple, and the footprint is small (around
12kb currently.)  libaeon is designed to be extremely simple to use, yet
still attempt to keep embedded and sbc developers in mind by being flexible,
lightweight, and efficient.

## Installation

```
./configure
make
sudo make install
```

## Contributors

Let people know how they can dive into the project, include important links to things like issue trackers, irc, twitter accounts if applicable.

## License

libAeon is distributed under the [BSD 3-Clause license](https://opensource.org/licenses/BSD-3-Clause).
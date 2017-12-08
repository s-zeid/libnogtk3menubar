libnogtk3menubar
================

An LD\_PRELOAD hack to hide the menu bar in GTK+ 3 apps.

Copyright (C) 2015, 2017 Scott Zeid.  Released under the X11 License.  
<https://code.s.zeid.me/libnogtk3menubar>


Usage
-----

Simply build libnogtk3menubar (see below) and set the `LD_PRELOAD` environment
variable to the path to the compiled library when running your program:

    LD_PRELOAD=path/to/libnogtk3menubar.so <GTK+ 3 program> [arguments [...]]

For convenience, a `run` script is provided that looks in the directory in which
itself is located for the library:

    path/to/libnogtk3menubar/run <program> [arguments [...]]


Building
--------

1.  Make sure you're on GNU/Linux (yes, the GNU part matters here) and have
    the glibc and GTK+ 3 development packages installed, as well as GNU Make
    and pkg-config.
    
        fedora$ sudo yum install glibc-devel gtk3-devel make pkgconfig

2.  In the repository's root directory, run `make`.

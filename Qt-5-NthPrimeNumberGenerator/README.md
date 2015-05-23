
This project was originally made with
QtCreator 3.3.2 Community Edition (Qt 5.4.1)

This should compile as soon as you import the qt project.

### UPDATE

Now this can be compiled with cmake.

Library dependencies:
~~~~~~~
sudo apt-get install libqt4-dev
~~~~~~~

To build (default compiler):
~~~~~~~
mkdir build
cd build
cmake ..
make
~~~~~~~

To use specific compiler (for instance, clang++-3.5):
~~~~~~~
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++-3.5 ..
make
~~~~~~~



MY WHINING:
~~~~~~~
It's ok to build it as long as you're in QtCreator
comfort zone with nice GUI designer and stuff
and everything just worked from the first time.

Compiles and runs, no problem. As soon as it got copied
to this repo and reopened it stops compiling.

Qt creator can't even find it's own headers after moving project?
REALLY? I guess that's why I prefer cmake with vim -
no obscure retarded homegrown project file formats
which may or may not work depending on the retardation
level of it's developers.

Good job Qt!

My rating of ease of use when creating this project: 6.2

UPDATE: I compiled this with cmake, so, +2 for rating: 8.2
~~~~~~~

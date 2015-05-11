This repository contains examples of loose coupling
using virtual packs from templatious library.

The program is simple - calculate nth prime number
asynchronously from gui in separate thread while updating
the progress.

This is made with following GUI frameworks:
Qt
gtkmm
wxWidgets
Ultimate++

The point is, four files across these projects are identical
(didn't need to be changed from project to project)

mainwindow_interface.h
messeagable.h
domain.h
domain.cpp

And this epic unix one liner executed from this directory proves that:

~~~~~~~
find . | grep -Ei '\.(cpp|hpp|h)' | xargs md5sum | grep -e domain -e messeagable -e mainwindow_interface | sort
~~~~~~~

Examples also contain README.md files which (hopefully) have examples on how to build these
projects to play around with them on your own. If you want to start fast without downloading
IDE's like QtCreator, CodeBlocks, Ultimate++ IDE I suggest checking out gtk example
first which builds with plain cmake.

Happy hacking!


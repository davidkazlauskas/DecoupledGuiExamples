
Project depents on cmake and gtk mm library:
~~~~~~~
sudo apt-get install libgtkmm-3.0-dev
~~~~~~~

TEMPLATIOUS_DIR enviroment variable should be defined
and point to templatious library.

To compile simply run (linux ubuntu 14.04):

~~~~~~~
mkdir build
cd build
cmake ..
make
~~~~~~~






MY WHINING:
~~~~~~~
First time using gtk was somewhat tricky and quirky.
I used glade designer to make ui.glade file, then
was kinda stuck with it as I wasn't sure what to do
with it.

Then I found out gtkmm and about it's loader and
made a wrapper class.

After that, I gotta say gtk plays VERY NICE! I
can actually just use cmake to build this with standard
cmake find packages and stuff, which for me is a HUUUGE bonus.

Just load ui.glade file and off you go, now THAT'S a great
way to build stuff.

My rating of ease of use when creating this project: 8.9
~~~~~~~

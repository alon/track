#
# Makefile for track, a tracked vehicle mockup simulation based on ODE.
#
#

############# Change These

# logical choices: g++ g++-3.0
CXX=g++
#CXX=g++-2.95
#CXX=g++-3.0

#PROFILEFLAG=-pg -fprofile-arcs -ftest-coverage
#PROFILELINK=-pg -fprofile-arcs -ftest-coverage
PROFILEFLAG=
PROFILELINK=

############# Don't change anything else

# debug and profile
DEBUG =-g -ggdb -Wall -DDEBUG # -O
#DEBUG =
CXXBASEFLAGS = $(PROFILEFLAG) -I. $(DEBUG) -I.
CXXFLAGS = $(CXXBASEFLAGS)  $(ODEFLAGS)#-frtti
LINK = $(PROFILELINK) #-O #-lefence

############# Tresspassers Will Be Toad

# ODE Flags

#ODEFLAGS=-fno-rtti -fno-exceptions $(REAL)
ODEFLAGS=$(REAL)
ODELINK=$(LINK) -lode
DRAWLINK=-L . -L/usr/X11/lib -L/usr/X11R6/lib -ldrawstuff -lm -lX11 -lGL -lGLU
# -ldrawstuff -L/usr/X11/lib -L/usr/X11R6/lib -lm -lX11 -lGL -lGLU
# -ldl -lpthread -lXext
############# General Rules

# make this the first rule
all: track

# calculate dependencies. append to file (read man page for more details)
depend:
	makedepend -- $(CXXFLAGS) -- *.cpp

clean:
	rm -f *.o track

################################################################################
DESIGNOBJ=cardesign.o trackdesign.o plainwheeldesign.o twheeldesign.o wheeldesign.o linkdesign.o mboxlinkdesign.o linkboxdesign.o design.o
GENERALOBJ=error.o mydraw.o automate.o cconfig.o Simulation.o
CONSTRUCTIONOBJ=myobj.o boxobj.o wheel.o plainwheel.o twheel.o chain.o car.o
MAINOBJ=main.o
OBJ=$(MAINOBJ) $(DESIGNOBJ) $(GENERALOBJ) $(CONSTRUCTIONOBJ)

# track
track: $(OBJ)
	$(CXX) -o $@ $^ $(LINK) $(ODELINK) $(DRAWLINK)

test: app.o apphalf.o appdesign.o bladedesign.o

# DO NOT DELETE

Simulation.o: ./drawstuff/drawstuff.h ./drawstuff/version.h Simulation.h
Simulation.o: track.h myobj.h error.h car.h cardesign.h common.h
Simulation.o: movabledesign.h design.h trackdesign.h linkdesign.h
Simulation.o: wheeldesign.h wheel.h chain.h app.h appdesign.h bladedesign.h
Simulation.o: apphalf.h automate.h scripting.h cconfig.h linkboxdesign.h
Simulation.o: mboxlinkdesign.h plainwheeldesign.h plainwheel.h twheeldesign.h
Simulation.o: twheel.h mydraw.h tags.h keymap.h
app.o: app.h appdesign.h design.h cardesign.h common.h myobj.h error.h
app.o: movabledesign.h trackdesign.h linkdesign.h wheeldesign.h wheel.h
app.o: bladedesign.h apphalf.h
appdesign.o: appdesign.h design.h cardesign.h common.h myobj.h error.h
appdesign.o: movabledesign.h trackdesign.h linkdesign.h wheeldesign.h wheel.h
appdesign.o: bladedesign.h
apphalf.o: apphalf.h myobj.h error.h appdesign.h design.h cardesign.h
apphalf.o: common.h movabledesign.h trackdesign.h linkdesign.h wheeldesign.h
apphalf.o: wheel.h bladedesign.h
automate.o: automate.h Simulation.h track.h myobj.h error.h car.h cardesign.h
automate.o: common.h movabledesign.h design.h trackdesign.h linkdesign.h
automate.o: wheeldesign.h wheel.h chain.h app.h appdesign.h bladedesign.h
automate.o: apphalf.h mydraw.h
bladedesign.o: bladedesign.h design.h
boxobj.o: boxobj.h myobj.h error.h mydraw.h
car.o: cardesign.h common.h myobj.h error.h movabledesign.h design.h
car.o: trackdesign.h linkdesign.h wheeldesign.h wheel.h boxobj.h car.h
car.o: chain.h track.h app.h appdesign.h bladedesign.h apphalf.h
cardesign.o: cardesign.h common.h myobj.h error.h movabledesign.h design.h
cardesign.o: trackdesign.h linkdesign.h wheeldesign.h wheel.h track.h
cconfig.o: cconfig.h error.h
chain.o: ./drawstuff/drawstuff.h ./drawstuff/version.h chain.h myobj.h
chain.o: error.h trackdesign.h common.h movabledesign.h design.h linkdesign.h
chain.o: track.h mydraw.h
design.o: design.h
error.o: error.h
linkballdesign.o: linkballdesign.h common.h myobj.h error.h linkdesign.h
linkballdesign.o: design.h track.h
linkboxdesign.o: linkboxdesign.h common.h myobj.h error.h linkdesign.h
linkboxdesign.o: design.h track.h
linkdesign.o: linkdesign.h common.h myobj.h error.h design.h track.h
main.o: keymap.h tags.h error.h common.h cconfig.h myobj.h mydraw.h car.h
main.o: cardesign.h movabledesign.h design.h trackdesign.h linkdesign.h
main.o: wheeldesign.h wheel.h chain.h track.h app.h appdesign.h bladedesign.h
main.o: apphalf.h automate.h Simulation.h linkboxdesign.h mboxlinkdesign.h
main.o: plainwheeldesign.h plainwheel.h twheeldesign.h twheel.h
main.o: main_command.cpp
mboxlinkdesign.o: mboxlinkdesign.h common.h myobj.h error.h linkdesign.h
mboxlinkdesign.o: design.h track.h
mydraw.o: ./drawstuff/drawstuff.h ./drawstuff/version.h common.h mydraw.h
myobj.o: myobj.h error.h mydraw.h common.h
plainwheel.o: ./drawstuff/drawstuff.h ./drawstuff/version.h plainwheel.h
plainwheel.o: wheel.h myobj.h error.h track.h plainwheeldesign.h
plainwheel.o: wheeldesign.h design.h mydraw.h
plainwheeldesign.o: plainwheeldesign.h plainwheel.h wheel.h myobj.h error.h
plainwheeldesign.o: track.h wheeldesign.h design.h
trackdesign.o: trackdesign.h common.h myobj.h error.h movabledesign.h
trackdesign.o: design.h linkdesign.h track.h wheeldesign.h wheel.h
twheel.o: ./drawstuff/drawstuff.h ./drawstuff/version.h twheel.h wheel.h
twheel.o: myobj.h error.h track.h twheeldesign.h common.h wheeldesign.h
twheel.o: design.h mydraw.h
twheeldesign.o: twheeldesign.h common.h myobj.h error.h wheeldesign.h
twheeldesign.o: design.h wheel.h twheel.h track.h
wheel.o: ./drawstuff/drawstuff.h ./drawstuff/version.h wheel.h myobj.h
wheel.o: error.h mydraw.h track.h
wheeldesign.o: wheeldesign.h design.h wheel.h myobj.h error.h

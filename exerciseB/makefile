# Makefile
# Glenn G. Chappell
# 2 Oct 2013
#
# Makefile for GLUT programs; default assumes one C++ source file

GCCFLAGS = -ansi -pedantic -Wall -Wextra -Wconversion \
  -Wno-unused-parameter -std=c++11
# Enable most errors/warnings, but not those for unused parameters

GLFLAGS = -lGL -lGLU -lglut -lGLEW
# May need to add to above: -lGLEW

#%: %.cpp
#	g++ -o $@ $< $(GCCFLAGS) $(GLFLAGS)
#
texture:
	g++ -o $@ splinepatch.cpp $(GCCFLAGS) $(GLFLAGS) 

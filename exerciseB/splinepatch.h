/* Paul Gentemann 
 * Bucky Frost
 * CS 381 
 * Assignment 6
 * Exercise B
 * Description : Headers for splinepatch.cpp
 * 
 */

// OpenGL/GLUT includes - DO THESE FIRST
#include <cstdlib>       // Do this before GL/GLUT includes
using std::exit;
#ifndef __APPLE__
# include <GL/glew.h>
# include <GL/glut.h>    // Includes OpenGL headers as well
#else
# include <GLEW/glew.h>
# include <GLUT/glut.h>  // Apple puts glut.h in a different place
#endif
#ifdef _MSC_VER          // Tell MS-Visual Studio about GLEW lib
# pragma comment(lib, "glew32.lib")
#endif

#include "lib381/bitmapprinter.h" // For in app doc
#include "lib381/glslprog.h"      // For GLSL code-handling functions
#include "lib381/globj.h" 	  // For class Tex2D
#include "lib381/tshapes.h"	  // For shape drawing funcs
#include "lib381/rtt.h"	          // For rendering to a texture 

#include <string>
using std::string;
#include <iostream>
using std::cerr; using std::endl; using std::cout;
#include <sstream>
using std::ostringstream;
#include <iomanip>
using std::setprecision; using std::fixed;
#include <cmath>
using std::sin; using std::exp;

// Function prototypes
void documentation();
void waveFun(GLdouble *, int, int);
void drawBezierPatch(int, GLdouble *, int);
void myDisplay();
void myIdle();
void resetZoom();
void fixShaderFloat(GLfloat *);
void myKeyboard(unsigned char, int, int);
void myPassiveMotion(int, int);
void init();
void myReshape(int, int);

// Global variables
const int ESCKEY = 27;         // ASCII value of Escape
const int startwinsize = 600;  // Start window width & height (pixels)
int winw = 1, winh = 1;        // Window width, height (pixels)
bool help = false;
bool wireFrame;

// Rotation
double savetime;
double rotangleSide;
double rotangleVert;
const double rotspeed = 40.0;
bool rotL;
bool rotR;
bool rotU;
bool rotD;

// Shaders
string vshader1fname;          // Filename for vertex shader source
string fshader1fname;          // Filename for fragment shader source
GLhandleARB prog1;             // GLSL Program Object (main)
GLhandleARB prog2;             // GLSL Program Object (2side)
bool shaderbool1 = true;
GLfloat shaderfloat1 = 1.;

// Textures
const int IMG_WIDTH = 1024, IMG_HEIGHT = IMG_WIDTH;
RTT cube0;

// Camera 
GLdouble viewmatrix[16];       
int zoom;

// Wave/Spline Patches
int numsubdivs;                // Number of subdivisions for object
const int minsubdivs = 1;      //  Minumum of above
const int maxsubdivs = 50;     //  Maximum of above
const int PTS = 4;             // Number of control points in a patch
const int EDGES = 4;           // Sides in a patch
const int SIZE = PTS*EDGES*3;  // Size of Patch Arrays
bool wave;                     // Starts the wave propagation.
GLdouble modd;                 // Value for the waves in the Bezier patches

// Bezier Patches
GLdouble b1[SIZE] = {
    3.5,-3.0, 0.0,   2.5,-2.0, 0.0,   1.5,-1.0, 0.0,   1.5, 0.0, 0.0,
    2.5,-3.0, 0.0,   0.5,-2.0, 0.0,   0.5,-1.0, 0.0,   0.5, 0.0, 0.0,
   -2.5,-3.0, 0.0,  -0.5,-2.0, 0.0,  -0.5,-1.0, 0.0,  -0.5, 0.0, 0.0,
   -3.5,-3.0, 0.0,  -2.5,-2.0, 0.0,  -1.5,-1.0, 0.0,  -1.5, 0.0, 0.0};
GLdouble b2[SIZE] = {
    1.5, 0.0, 0.0,   1.5, 1.0, 0.0,   2.5, 2.0, 0.0,   3.5, 3.0, 0.0,
    0.5, 0.0, 0.0,   0.5, 1.0, 0.0,   0.5, 2.0, 0.0,   0.5, 3.0, 0.0,
   -0.5, 0.0, 0.0,  -0.5, 1.0, 0.0,  -0.5, 2.0, 0.0,  -0.5, 3.0, 0.0,
   -1.5, 0.0, 0.0,  -1.5, 1.0, 0.0,  -2.5, 2.0, 0.0,  -3.5, 3.0, 0.0};

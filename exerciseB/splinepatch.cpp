/* Authors: Bucky Frost
 * 	    Paul Gentemann
 * CS 381
 * File Name: splinepatch.cpp
 * Last Modified: Mon Dec  9 04:50:40 AKST 2013
 * Description: A pair of spline plains, patched together, that will
 *     make a wave upon user input that are textured. Borrows heavily
 *     from hw5 and some from sample Chappell code.
 */

#include "splinepatch.h"

// waveFun
// Makes a wave through one of the axes of a spline patch. 
void waveFun(GLdouble *arr, int column, int axis)
{
    for (int i = 0; i < PTS; ++i)
    {
        int pos = 3 * PTS * i + (3 * column + axis);
        arr[pos] = exp(1-modd) * sin(2 * 3.14 * modd); // Dampened wave.
    }
}

// drawBezierPatch
// Draws a number of control points for a bezier patch. The z coordinates 
// of all the points are translated by the sine of the mod parameter.
void drawBezierPatch(int subdivs, GLdouble *cpts)
{
    glColor3d(0.,0.,0.5);
    glMap2d(GL_MAP2_VERTEX_3, 0., 1., 3, 4, 0., 1., 3*4, 4, cpts);
    glEnable(GL_MAP2_VERTEX_3);

    glMapGrid2d(subdivs, 0., 1., subdivs, 0., 1.);
    glEnable(GL_AUTO_NORMAL);

    glFrontFace(GL_CW);  // Normals are evidently backwards here :-(
    wireFrame ? glEvalMesh2(GL_LINE, 0, subdivs, 0, subdivs) 
    	      : glEvalMesh2(GL_FILL, 0, subdivs, 0, subdivs);
    glFrontFace(GL_CCW);
}

// drawCutSphere
// Draws a sphere made of different-colored polygons with gaps
//  between them. Normals face inward. Borrowed from Chappell.
void drawCutSphere()
{
    const double pi = 3.1415926535898;
    const double smang = 0.01;
    const int slices = 32;
    const int stacks = 24;

    for (int i = 0; i < slices; ++i)
    {
        double ia1 = 2.*pi*double(i)/slices + smang;
        double ia2 = 2.*pi*double(i+1)/slices - smang;

        for (int k = 1; k < stacks-1; ++k)
        {
            double ka1 = pi*(double(k)/stacks-0.5) + smang;
            double ka2 = pi*(double(k+1)/stacks-0.5) - smang;

            glColor3d(0.4*cos(2*ia1)/2.+0.5, 0.5, 0.4*cos(4*ka1)/2.+0.5);
            glBegin(GL_QUADS);
                glNormal3d(-cos(ia1)*cos(ka1), -sin(ka1), -sin(ia1)*cos(ka1));
                glVertex3d( cos(ia1)*cos(ka1),  sin(ka1),  sin(ia1)*cos(ka1));
                glNormal3d(-cos(ia2)*cos(ka1), -sin(ka1), -sin(ia2)*cos(ka1));
                glVertex3d( cos(ia2)*cos(ka1),  sin(ka1),  sin(ia2)*cos(ka1));
                glNormal3d(-cos(ia2)*cos(ka2), -sin(ka2), -sin(ia2)*cos(ka2));
                glVertex3d( cos(ia2)*cos(ka2),  sin(ka2),  sin(ia2)*cos(ka2));
                glNormal3d(-cos(ia1)*cos(ka2), -sin(ka2), -sin(ia1)*cos(ka2));
                glVertex3d( cos(ia1)*cos(ka2),  sin(ka2),  sin(ia1)*cos(ka2));
            glEnd();
        }
    }
}

// Function to draw teapots at various locations on the screen.
void drawTeaPot(GLdouble tx, GLdouble ty, GLdouble tz,
                GLdouble sx, GLdouble sy, GLdouble sz,
                GLdouble r, GLdouble g, GLdouble b, GLdouble size)
{
    glPushMatrix();
        glTranslated(tx, ty, tz);
        glScaled(sx, sy, sz);
        glColor3d(r, g, b);
        glFrontFace(GL_CW); // Teapot has stupid facing
        glutSolidTeapot(size);
        glFrontFace(GL_CCW);
    glPopMatrix();
}

// Draws the actual, unlit, objects in the scene.
void drawSurroundings()
{
    // CHOOSE PROGRAM OBJECT
    GLhandleARB theprog;  // CURRENTLY-used program object or 0 if none
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    theprog = 0;

    // Initialize buffer
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Draw background sphere
    glPushMatrix();
    glScaled(10., 10., 10.);
    drawCutSphere();
    glPopMatrix();

    // Position light source 0 & draw ball there
    // Also give spot direction
    glPushMatrix();
    glTranslated(-1., 1., 3.);
    GLfloat origin4[] = { 0.f, 0.f, 0.f, 1.f };
    glLightfv(GL_LIGHT0, GL_POSITION, origin4);
    GLfloat spotdir[] = { 1.f, -1.f, -1.f };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotdir);
    glColor3d(1., 1., 1.);
    glutSolidSphere(0.1, 20, 15);
    glPopMatrix();

    // LIT OBJECTS
    // Make program object (if any) active
    glUseProgramObjectARB(theprog);

    // Send values to shaders
    if (theprog)
    {
        GLint loc;  // Location for shader vars

        loc = glGetUniformLocationARB(theprog, "myb1");
        if (loc != -1)
            glUniform1i(loc, true);

        loc = glGetUniformLocationARB(theprog, "myf1");
        if (loc != -1)
            glUniform1f(loc, 1.f);

        // Send texture channels
        loc = glGetUniformLocationARB(theprog, "mycube0");
        if (loc != -1)
            glUniform1i(loc, 0);
    }

    // Draw left side object
    drawTeaPot(-2.5, 0.0,-5.0,   0.5, 0.5, 0.5,   0.0, 0.5, 1.0,   1.0);
    drawTeaPot( 2.5, 0.0,-5.0,   0.5, 0.5, 0.5,   0.5, 0.0, 0.8,   1.0);
    drawTeaPot( 0.0, 0.5,-5.0,   0.5, 0.5, 0.5,   0.5, 0.8, 0.0,   1.0);
}

void makeTextures()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(90., 1., 0.1, 10.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glMultMatrixd(viewmatrix);

    // +x face
    glPushMatrix();
    glMultMatrixd(viewmatrix);
    glRotated(180., 0.,0.,1.);
    glRotated(90., 0.,1.,0.);
    cube0.beginRender(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    drawSurroundings();
    cube0.endRender(false);
    glPopMatrix();

    // -x face
    glPushMatrix();
    glMultMatrixd(viewmatrix);
    glRotated(180., 0.,0.,1.);
    glRotated(-90., 0.,1.,0.);
    cube0.beginRender(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    drawSurroundings();
    cube0.endRender(false);
    glPopMatrix();

    // +y face
    glPushMatrix();
    glMultMatrixd(viewmatrix);
    glRotated(-90., 1.,0.,0.);
    cube0.beginRender(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    drawSurroundings();
    cube0.endRender(false);
    glPopMatrix();

    // -y face
    glPushMatrix();
    glMultMatrixd(viewmatrix);
    glRotated(90., 1.,0.,0.);
    cube0.beginRender(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    drawSurroundings();
    cube0.endRender(false);
    glPopMatrix();

    // +z face
    glPushMatrix();
    glMultMatrixd(viewmatrix);
    glRotated(180., 0.,0.,1.);
    glRotated(180., 0.,1.,0.);
    cube0.beginRender(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    drawSurroundings();
    cube0.endRender(false);
    glPopMatrix();

    // -z face
    glPushMatrix();
    glMultMatrixd(viewmatrix);
    glRotated(180., 0.,0.,1.);
    cube0.beginRender(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    drawSurroundings();
    cube0.endRender();
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


// myDisplay
// The GLUT display function
void myDisplay()
{
    // Update the environment map
    makeTextures();

    glEnable(GL_DEPTH_TEST);    // Set up 3D
    glLoadIdentity();           // Start with camera.
    glMultMatrixd(viewmatrix);

    glTranslated(0,0, -zoom);   // Position objects out at distance
    drawSurroundings();     // Draw all non-reflection-mapped objs.
    GLhandleARB theprog;    // Currently-used program obj. or 0.

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    theprog = prog1;


    glUseProgramObjectARB(theprog);
        glRotated(rotangleSide, 0., 1., 0.);
	glRotated(rotangleVert, 1.0, 0.0, 0.0);
    
    // Draw Objects
    if(wave)
    {
        modd += 0.1;
        waveFun(b1, 3, 2);  // Last set of points, using z-coords
        waveFun(b1, 2, 2);  // Second to last set of points.
        waveFun(b2, 0, 2);  // First points to second patch.
        waveFun(b2, 1, 2);  // Second set of points.
    }

    drawBezierPatch(numsubdivs, b1);
    drawBezierPatch(numsubdivs, b2);
    documentation();    

    glutSwapBuffers();
}

// myIdle
// The GLUT idle function
void myIdle()
{
    // Compute elapsed time since last movement
    double currtime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double elapsedtime = currtime - savetime;
    savetime = currtime;
    if (elapsedtime > 0.1)
    	elapsedtime = 0.1;
    
    // Rot objs
    if (rotL)
    {
        rotangleSide += rotspeed * elapsedtime;
        glutPostRedisplay();    
    }
    if (rotR)
    {
        rotangleSide -= rotspeed * elapsedtime;
        glutPostRedisplay();
    }
    if (rotU)
    {
        rotangleVert += rotspeed * elapsedtime;
        glutPostRedisplay();
    }
    if (rotD)
    {
        rotangleVert -= rotspeed * elapsedtime;
        glutPostRedisplay();
    }

    // Print OpenGL errors, if there are any (for debugging)
    static int error_count = 0;
    if (GLenum err = glGetError())
    {
        ++error_count;
        cerr << "OpenGL ERROR " << error_count << ": "
             << gluErrorString(err) << endl;
    }
}

// fixShaderFloat
// Makes sure that the shader float does not exceed the bounds [0,1].
void fixShaderFloat(GLfloat *f)
{
    if (*f < 0.) *f = 0.;
    if (*f > 1.) *f = 1.;
}

// resetZoom
// Resets the initial zoom value (use everywhere for DRY)
void resetZoom()
{
    zoom = 5;
    glLoadIdentity();
    glGetDoublev(GL_MODELVIEW_MATRIX, viewmatrix);
}

// myKeyboard
// The GLUT keyboard function
void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ESCKEY:  // Esc: quit
        exit(0);
        break;
    case '+':
        --zoom;
        break;
    case '-':
        ++zoom;
        break;
    case 'R':
    case 'r':
        resetZoom();
        break;
    case ' ':
        wave = true;
        modd = 0.;
        break;
    case '(':
        if(numsubdivs > minsubdivs)
            --numsubdivs;
        break;
    case ')':
        if(numsubdivs < maxsubdivs)
            ++numsubdivs;
        break;
    case 'w':
    case 'W':
    	wireFrame = !wireFrame;
	break;
    case 'h':
    case 'H':
        help = !help;
        break;
    case 'F':
    case 'f':     // Space: toggle shader bool
        shaderbool1 = !shaderbool1;
        break;
    case '[':     // [: Decrease shader float
        shaderfloat1 -= 0.02;
        fixShaderFloat(&shaderfloat1);
        break;
    case ']':     // ]: Increase shader float
        shaderfloat1 += 0.02;
        fixShaderFloat(&shaderfloat1);
        break;
    }
    glutPostRedisplay();
}

// mySpecial
// The GLUT special function
void mySpecial(int key, int x, int y)
{
    glTranslated(0., 0., -zoom);
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotL = !rotL;
	if (rotR)
	    rotR = !rotR;
        break;
    case GLUT_KEY_RIGHT:
        rotR = !rotR;
        if (rotL)
            rotL = !rotL;
        break;
    case GLUT_KEY_UP:
        rotU = !rotU;
        if (rotD)
            rotD = !rotD;
        break;
    case GLUT_KEY_DOWN:
        rotD = !rotD;
        if (rotU)
            rotU = !rotU;
        break;
    }
    glTranslated(0., 0., zoom);
    glMultMatrixd(viewmatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, viewmatrix);

    glutPostRedisplay();
}

// myReshape
// The GLUT reshape function
void myReshape(int w, int h)
{
    // Set viewport & save window dimensions in globals
    glViewport(0, 0, w, h);
    winw = w;
    winh = h;
    // Set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60., double(w)/h, 0.01, 100.);
    glMatrixMode(GL_MODELVIEW);  // Always go back to model/view mode
}

// documentation
void documentation()
{
    // Draw documentation
    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0., double(winw), 0., double(winh));
        glColor3d(0., 0., 0.);        // Black text
        BitmapPrinter p(20., winh - 20., 20.);
        if(help)
        {
            ostringstream os1;
            ostringstream os2;
            os1 << fixed << setprecision(2) << shaderfloat1;
            os2 << fixed << setprecision(2) << numsubdivs;
            p.print("Arrows         Rotate Plane");
            p.print("[ ]            Change Lighting (" + os1.str() + ")");
            p.print("( )            Change Subdivisions (" + os2.str() + ")");
            p.print("+/-            Zoom in/out");
            p.print("r              Reset Camera");
            p.print("space          Distort Spline");
            p.print(string("w              Wire-Frame (" )
	    		+ (wireFrame ? "true" : "false") + ")");
            p.print(string("f              Lighting (" )
                        + (shaderbool1 ? "true" : "false") + ")");
        }
        else
        {
            p.print("h              help");
        }
        p.print("Esc            Quit");
    glPopMatrix();                // Restore prev projection
    glMatrixMode(GL_MODELVIEW);
}

// init
// Initialize GL states & global data
void init()
{
    resetZoom();    // Reset camera position to default.
    glMatrixMode(GL_MODELVIEW);
    modd = 0.0;
    wave = false;
    numsubdivs = 10;
    wireFrame = false;
    
    // init rotation
    rotL = true;
    rotR = false;
    rotU = false;
    rotD = false;

    rotangleSide = 0.0;
    rotangleVert = 0.0;
    
    // Texture
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Shaders
    prog1 = makeProgramObjectFromFiles(vshader1fname, fshader1fname); 
    prog2 = makeProgramObjectFromFiles("twoside_v.glsl", "twoside_f.glsl");

    // Textures

    // Make Texture 0
    glActiveTexture(GL_TEXTURE0);
    cube0.init(GL_TEXTURE_CUBE_MAP, IMG_WIDTH, IMG_HEIGHT);

    // Set params
    cube0.bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,
        GL_TEXTURE_WRAP_T, GL_CLAMP);
}

// The main
int main(int argc, char ** argv)
{
    // Initilization of OpenGL/GLUT
    glutInit(&argc, argv);
    // Set shader source filenames. Done here, as opposed to init() so we can 
    // use command line arguments.
    getShaderFilenames(vshader1fname, fshader1fname, argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Creating the view window
    glutInitWindowSize(startwinsize, startwinsize);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS 381 - Wobbly Goodness!");

    // Init GLEW & check status - exit on failure
    if (glewInit() != GLEW_OK)
    {
        cerr << "glewInit failed" << endl;
        exit(1);
    }

    init();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);

    // Start GLUT event handling loop
    glutMainLoop();

    return 0;
}


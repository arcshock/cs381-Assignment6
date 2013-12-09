// whatever_tex_f.glsl
// Glenn G. Chappell
// 1 Nov 2012
//
// For CS 381 Fall 2013
// GLSL Fragment Shader for use with usetextures.cpp
// Whatever I Feel Like Doing with Textures

uniform float myf1;        // Application float (in [0.,1.]; start: 1.)
uniform bool myb1;         // Application bool (start: true)

varying vec2 mytexcoord0;  // 2-D texture coordinates for texture 0
varying vec2 mytexcoord1;  // 2-D texture coordinates for texture 1
uniform sampler2D mytex0;  // 2-D texture 0
uniform sampler2D mytex1;  // 2-D texture 1


void main()
{
    // Get colors from textures
    vec4 color0 = texture2D(mytex0, mytexcoord0);
    vec4 color1 = texture2D(mytex1, mytexcoord1);

    // Discard if high blue component
    if (myb1 && color0.b > 0.6)
        discard;

    // Set fragment color, changing it for a backface.
    vec4 thecolor = mix(color1, color0, myf1);
    if (!gl_FrontFacing)
    {
        thecolor.b += 0.2;
        if (thecolor.b > 1.)
            thecolor.b -= 1.;
    }
    gl_FragColor = thecolor;
}


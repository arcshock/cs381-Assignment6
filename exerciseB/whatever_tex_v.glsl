// whatever_tex_v.glsl
// Glenn G. Chappell
// 1 Nov 2012
//
// For CS 381 Fall 2013
// GLSL Vertex Shader for use with usetextures.cpp
// Whatever I Feel Like Doing with Textures

varying vec2 mytexcoord0;  // 2-D texture coordinates for texture 0
varying vec2 mytexcoord1;  // 2-D texture coordinates for texture 1


void main()
{
    // Transform the vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Texture coordinates 0: transform
    vec4 texcoord4 = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    mytexcoord0 = texcoord4.st / texcoord4.q;
    mytexcoord0 *= 2.;  // Shrink textures

    // Texture coordinates 1: no transform
    mytexcoord1 = gl_MultiTexCoord0.st / gl_MultiTexCoord0.q;
}


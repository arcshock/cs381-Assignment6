// whatever_tex_v.glsl
// Bucky Frost and Paul Gentemann
// Dec 4, 2013
//
// For CS 381 Fall 2013
// GLSL Vertex Shader for use with usetextures.cpp
// glassy

varying vec3 surfpt;       // Point on surface (camera coords)
varying vec3 surfnorm_un;  // Surface normal (camera coords)

void main()
{
    // Transform the vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Find object position (camera coords)
    vec4 surfpt4 = gl_ModelViewMatrix * gl_Vertex;
    surfpt = surfpt4.xyz / surfpt4.w;

    // Transform normal vector and ensure it has length 1
    surfnorm_un = normalize(gl_NormalMatrix * gl_Normal);
}


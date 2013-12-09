// whatever_tex_f.glsl
// Bucky Frost and Paul Gentemann
// Dec 4, 2013
// CS 381 

// GLSL Fragment Shader for use with usetextures.cpp
// Glass?

uniform float myf1;        // Application float (in [0.,1.]; start: 1.)
uniform bool myb1;         // Application bool (start: true)

varying vec3 surfpt;       // Point on surface (camera coords)
varying vec3 surfnorm_un;  // Surface normal (camera coords)
uniform samplerCube mycube0; // A cube-map texture
                           

void main()
{
    // Find our normal vector & viewing direction
    vec3 surfnorm = normalize(surfnorm_un);
    if (!gl_FrontFacing)
    {
        surfnorm = -surfnorm;
    }
    vec3 viewdir = normalize(surfpt);

    // *** Reflection mapping ***
    vec3 reflectview = normalize(reflect(viewdir, surfnorm));
    vec4 reflectmapcolor = textureCube(mycube0, reflectview);

    // *** Refraction mapping ***
    float indexbase = 1.1;            // Base refractive index
    float indexdelta = 0.005 * myf1;  // Change in refractive index by
                                      //  frequency; for chromatic
                                      //  aberration
                                      
    vec4 refractmapcolor = vec4(0., 0., 0., 1.);

    // Red
    float index_r = indexbase - indexdelta;
    vec3 refractview_r = normalize(refract(viewdir, surfnorm, 1./index_r));
    refractmapcolor.r = textureCube(mycube0, refractview_r).r;

    // Green
    float index_g = indexbase;
    vec3 refractview_g = normalize(refract(viewdir, surfnorm, 1./index_g));
    refractmapcolor.g = textureCube(mycube0, refractview_g).g;

    // Blue
    float index_b = indexbase + indexdelta;
    vec3 refractview_b = normalize(refract(viewdir, surfnorm, 1./index_b));
    refractmapcolor.b = textureCube(mycube0, refractview_b).b;

    // *** Final color ***
    // Mix refraction & reflection according to how close viewing dir
    //  is to the normal.
    float t = max(0., dot(-viewdir, surfnorm));
    if (!myb1)
        t = 0.8;
    gl_FragColor = mix(reflectmapcolor, refractmapcolor, t);
}


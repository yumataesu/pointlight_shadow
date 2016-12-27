#version 330 core

uniform float far;
uniform vec3 lightPos;

in vec4 FragPos;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far;
    
    // Write this as modified depth
    gl_FragDepth = lightDistance;
}

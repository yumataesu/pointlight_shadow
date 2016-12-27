#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

struct Light {
    mat4 viewProjectionMatirx;
};
uniform Light light[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; face++)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        
        {
            FragPos = gl_in[0].gl_Position;
            gl_Position = light[face].viewProjectionMatirx * FragPos;
            EmitVertex();
        }
        
        
        {
            FragPos = gl_in[1].gl_Position;
            gl_Position = light[face].viewProjectionMatirx * FragPos;
            EmitVertex();
        }
        
        
        {
            FragPos = gl_in[2].gl_Position;
            gl_Position = light[face].viewProjectionMatirx * FragPos;
            EmitVertex();
        }
    
        EndPrimitive();
    }
}

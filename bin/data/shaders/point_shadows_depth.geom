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
    
    gl_Layer = 0;
    FragPos = gl_in[0].gl_Position;
    gl_Position = light[0].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[1].gl_Position;
    gl_Position = light[0].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[2].gl_Position;
    gl_Position = light[0].viewProjectionMatirx * FragPos;
    EmitVertex();
    EndPrimitive();
    
        
    gl_Layer = 1;
    FragPos = gl_in[0].gl_Position;
    gl_Position = light[1].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[1].gl_Position;
    gl_Position = light[1].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[2].gl_Position;
    gl_Position = light[1].viewProjectionMatirx * FragPos;
    EmitVertex();
    EndPrimitive();
    
    
    gl_Layer = 2;
    FragPos = gl_in[0].gl_Position;
    gl_Position = light[2].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[1].gl_Position;
    gl_Position = light[2].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[2].gl_Position;
    gl_Position = light[2].viewProjectionMatirx * FragPos;
    EmitVertex();
    EndPrimitive();
    
    

    gl_Layer = 3;
    FragPos = gl_in[0].gl_Position;
    gl_Position = light[3].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[1].gl_Position;
    gl_Position = light[3].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[2].gl_Position;
    gl_Position = light[3].viewProjectionMatirx * FragPos;
    EmitVertex();
    EndPrimitive();
    
    
    gl_Layer = 4;
    FragPos = gl_in[0].gl_Position;
    gl_Position = light[4].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[1].gl_Position;
    gl_Position = light[4].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[2].gl_Position;
    gl_Position = light[4].viewProjectionMatirx * FragPos;
    EmitVertex();
    EndPrimitive();

    
    gl_Layer = 5;
    FragPos = gl_in[0].gl_Position;
    gl_Position = light[5].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[1].gl_Position;
    gl_Position = light[5].viewProjectionMatirx * FragPos;
    EmitVertex();
    
    FragPos = gl_in[2].gl_Position;
    gl_Position = light[5].viewProjectionMatirx * FragPos;
    EmitVertex();
    EndPrimitive();
}

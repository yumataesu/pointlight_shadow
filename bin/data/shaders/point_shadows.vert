#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 TexCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    
    vec3 toLightVector;
} vs_out;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;
uniform mat4 lightViewProjection;


uniform int debug;
uniform bool reverse_normals;

void main()
{
    
    if(debug == 1)
    {
        gl_Position = lightViewProjection * model * vec4(position, 1.0f);
    }
    
    else
    {
        gl_Position = projection * view * model * vec4(position, 1.0f);
    }
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    
    
    vs_out.toLightVector = lightPos - vs_out.FragPos;
    
    if(reverse_normals) // A slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
        vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * normal);
    else
        vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoord = texcoord;
}

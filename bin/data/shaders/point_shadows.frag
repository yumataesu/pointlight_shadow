#version 330 core


in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    
    vec3 toLightVector;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthCubemap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far;
bool shadows = true;


out vec4 FragColor;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
   vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
   vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
   vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    
    // Use the fragment to light vector to sample from the depth map
    //float closestDepth = texture(depthCubemap, fragToLight).r;
    
    // It is currently in linear range between [0,1]. Let's re-transform it back to original depth value
    //closestDepth *= far;
    
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    
    // Now test for shadows
    // float bias = 0.05; // We use a much larger bias since depth is now in [near_plane, far] range
    // float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    // PCF
    // float shadow = 0.0;
    // float bias = 0.05;
    // float samples = 4.0;
    // float offset = 0.1;
    // for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    // {
    // for(float y = -offset; y < offset; y += offset / (samples * 0.5))
    // {
    // for(float z = -offset; z < offset; z += offset / (samples * 0.5))
    // {
    // float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; // Use lightdir to lookup cubemap
    // closestDepth *= far;   // Undo mapping [0;1]
    // if(currentDepth - bias > closestDepth)
    // shadow += 1.0;
    // }
    // }
    // }
    // shadow /= (samples * samples * samples);
    
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthCubemap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    
    // Display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3(closestDepth / far), 1.0);
    
    // return shadow;
    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoord).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    
    // Ambient
    vec3 ambient = 0.7 * color;
    
    // Diffuse
    vec3 lightDir = normalize(fs_in.toLightVector);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    
    
    //Attenuation
    float dist = length(fs_in.toLightVector) / 100.0f;
    float attenuation = 1.0 + (0.1 * dist) + (1.8 * dist * dist);
    
    ambient /= attenuation;
    diffuse /= attenuation;
    specular /= attenuation;

    
    // Calculate shadow
    float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    
    FragColor = vec4(lighting, 1.0f);
}

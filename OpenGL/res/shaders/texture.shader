#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_LightDirection;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform vec3 u_LightDirection;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    vec4 eyeSpacePosition = u_Model * vec4(a_Position, 1.0);
    vec3 eyeSpaceLightDir = mat3(u_Model) * u_LightDirection;
    v_LightDirection = normalize(eyeSpaceLightDir - eyeSpacePosition.xyz);
}

#shader fragment
#shader fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor; // Add the uniform for light color

void main()
{
    FragColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.5);
    FragColor.rgb *= u_LightColor; // Apply light color to the texture
}

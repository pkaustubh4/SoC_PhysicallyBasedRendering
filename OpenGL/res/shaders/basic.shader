#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoord;

uniform mat4 u_MVP;

out vec2 v_TextureCoord;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);
    v_TextureCoord = a_TextureCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_color;

void main() {
   color = u_color;
}

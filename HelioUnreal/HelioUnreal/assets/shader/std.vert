#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 projMatrix, viewMatrix;

in vec4 position;
in vec4 color;
in vec2 texCoord;

out vec4 vColor;
out vec2 vTexCoord;

void main()
{
    gl_Position = projMatrix * viewMatrix * position;
    vColor = color;
    vTexCoord = texCoord;
}

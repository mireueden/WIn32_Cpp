#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex;

in vec4 vColor;
in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
    fragColor = texture(tex, vTexCoord) * vColor;
}

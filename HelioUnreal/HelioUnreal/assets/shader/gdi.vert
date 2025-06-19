#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 projMatrix, viewMatrix;

in vec4 position;

void main()
{
    gl_Position = projMatrix * viewMatrix * position;
}
// =====================================
//             번역기 prgram
// cpu     (vert + frag) shader      gpu

// cpu => vert => frag => gpu
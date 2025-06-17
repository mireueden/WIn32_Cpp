#version 150

#ifdef GL_ES
precision mediump float;
#endif

// 보간 필요없음 : uniform
uniform mat4 projMatrix, viewMatrix;

// 보간 계산 필요 : Attr
in vec4 position;
                            
void main() 
{               
     gl_Position = projMatrix * viewMatrix * position;
}                           

#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 projMatrix, viewMatrix;

in vec4 position;
in vec4 color;
in vec4 texCoord;
in vec4 normal;

void main()
{
    gl_Position = projMatrix * viewMatrix * position;
}                        

// glsl
// 파이프라인 구조체?
// cpu가 gpu에 명령 : 명령을 위해서 변역기 필요
//          번역기 Program
// cpu      =(vert + flag)Shader=>      cpu
// cpu => vert => frag => gpu
// cpu : Attr로 position을 vert에 전달
// vert : in으로 cpu의 poisition값을 받고, out으로 vPosition값을 frag에 전달
// frag : in으로 vert의 vPosition을 받고, out으로 fragColor값을 gpu에 전달

// gl_Position : 
// gl_FragCoord : 물리적인 크기의 해상도

// attrbute(어트리뷰트)
// 각 점마다 그 사이에 잇는걸 모두 들고 올 수 없으니까
// 두개의 점을 지정하면 그 사이에 대한걸 기본적으로 들고 오게?
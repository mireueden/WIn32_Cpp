#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec3      iResolution;          
uniform float     iTime;                
uniform float     iTimeDelta;           
uniform float     iFrameRate;           
uniform int       iFrame;               
uniform float     iChannelTime[4];      
uniform vec3      iChannelResolution[4];
uniform vec4      iMouse;               
uniform sampler2D iChannel0;            
uniform sampler2D iChannel1;            
uniform sampler2D iChannel2;            
uniform sampler2D iChannel3;            
uniform vec4      iDate;                
uniform float     iSampleRate;          
                                        
out vec4 fragColor;                     

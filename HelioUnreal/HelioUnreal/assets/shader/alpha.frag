#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex;
uniform float iTime;

in vec4 vColor;
in vec2 vTexCoord;

out vec4 fragColor;         
             

void main() 
{    
    vec4 c = texture(tex,vTexCoord)* vColor;
	float grey = c.r * 0.3 + c.g * 0.5 + c.b * 0.2;

	float a= sin(iTime);
	vec3 rgb = c.rgb* (1.-1) + vec3(grey)* a;

	fragColor = vec4(rgb, c.a*vColor);
}                           



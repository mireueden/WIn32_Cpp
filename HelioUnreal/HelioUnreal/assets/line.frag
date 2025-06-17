#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_start;
uniform vec2 u_end;
uniform float u_width;
uniform vec4 u_color;

out vec4 fragColor;         
              
float getDistance(vec2 p, vec2 s, vec2 e, float width)
{
	vec2 m = p - s;
	vec2 n = e - s;
	float len = length(n);
	n /= len;

	vec2 proj = clamp(dot(m, n), 0.0, len) * n;

	return length(m - proj) - width / 2.;
}


void main() 
{           
	float d = getDistance(gl_FragCoord.xy, u_start,u_end,u_width);
	float a = clamp(-d, 0.0, 0.1);

    fragColor = vec4(u_color.rgb, u_color.a * a);
}                           



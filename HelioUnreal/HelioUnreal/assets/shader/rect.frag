#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 u_rect;
uniform vec4 u_color;
uniform float u_radius;

out vec4 fragColor;

float getDistance(vec2 p, vec2 size, float radius)
{
	size -= vec2(radius);
	vec2 d = abs(p) - size;
	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

void main()
{
	float d = getDistance(gl_FragCoord.xy - u_rect.xy, u_rect.zw, u_radius);
	float a = clamp(-d, 0.0, 1.0);
    fragColor = vec4(u_color.rgb, u_color.a * a);
}

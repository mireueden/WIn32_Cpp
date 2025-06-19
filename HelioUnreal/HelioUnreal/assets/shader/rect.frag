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
	float a = clamp(abs(d), 0.0, 1.0);
    fragColor = vec4(u_color.rgb, u_color.a * a);
}

// cpu에서 값이 제대로 설정
// cpu에서 gpu데이터 넘길때, 값이 제대로 전달(attr, uniform)
// 쉐이더에서 값을 제대로 사용


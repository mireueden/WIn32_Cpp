void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float d = length(fragCoord.xy - iResolution.xy / 2.);
    float d2 = iResolution.x * 0.7 * abs(sin(iTime));
    float a = clamp(d - d2, 0.0f, 1.0);
    fragColor = texture(iChannel0, fragCoord/iResolution.xy) * (1.0- a) + vec4(0.) * a;
}

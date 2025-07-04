#if 1
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float d = length(fragCoord.xy - iResolution.xy / 2.);
    float d2 = iResolution.x * 0.7 * abs(sin(iTime));
    float a = clamp(d - d2, 0.0f, 1.0);
    fragColor = texture(iChannel0, fragCoord/iResolution.xy) * (1.0- a) + vec4(0.) * a;
}
#elif 0
void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;
    float bias = abs(sin(iTime))*6.;
    fragColor = texture(iChannel0, uv, bias);
}
#else
const int   c_samplesX    = 15;  // must be odd
const int   c_samplesY    = 15;  // must be odd
const float c_textureSize = 512.0;

const int   c_halfSamplesX = c_samplesX / 2;
const int   c_halfSamplesY = c_samplesY / 2;
const float c_pixelSize = (1.0 / c_textureSize);

float Gaussian (float sigma, float x)
{
    return exp(-(x*x) / (2.0 * sigma*sigma));
}

vec3 BlurredPixel (in vec2 uv, float bias)
{
    float c_sigmaX = bias;//iMouse.z > 0.0 ? 5.0 * iMouse.x / iResolution.x : (sin(iTime*2.0)*0.5 + 0.5) * 5.0;
	float c_sigmaY = bias;//iMouse.z > 0.0 ? 5.0 * iMouse.y / iResolution.y : c_sigmaX;
    
    float total = 0.0;
    vec3 ret = vec3(0);
        
    for (int iy = 0; iy < c_samplesY; ++iy)
    {
        float fy = Gaussian (c_sigmaY, float(iy) - float(c_halfSamplesY));
        float offsety = float(iy-c_halfSamplesY) * c_pixelSize;
        for (int ix = 0; ix < c_samplesX; ++ix)
        {
            float fx = Gaussian (c_sigmaX, float(ix) - float(c_halfSamplesX));
            float offsetx = float(ix-c_halfSamplesX) * c_pixelSize;
            total += fx * fy;            
            ret += texture(iChannel0, uv + vec2(offsetx, offsety)).rgb * fx*fy;
        }
    }
    return ret / total;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy * vec2(1,-1);
    float bias = 5.0 * abs(sin(iTime));
	fragColor = vec4(BlurredPixel(uv, bias), 1.0);
}
#endif

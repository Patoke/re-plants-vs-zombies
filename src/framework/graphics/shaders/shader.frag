#version 450

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec4 fragColor;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    vec4 vertices[4];
    uint colors[4];
    vec4 renderColor;
    bool isQuad;
    bool toFilter;
} PushConstants;


// from http://www.java-gaming.org/index.php?topic=35123.0
vec4 cubic(float v) {
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    float x = s.x;
    float y = s.y - 4.0 * s.x;
    float z = s.z - 4.0 * s.y + 6.0 * s.x;
    float w = 6.0 - x - y - z;
    return vec4(x, y, z, w) * (1.0/6.0);
}

vec4 textureBicubic(sampler2D samp, vec2 texCoords) {
    vec2 texSize = textureSize(samp, 0);
    vec2 invTexSize = 1.0 / texSize;

    texCoords = texCoords * texSize - 0.5;

    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    vec4 xcubic = cubic(fxy.x);
    vec4 ycubic = cubic(fxy.y);

    vec4 c = texCoords.xxyy + vec2 (-0.5, +1.5).xyxy;
    
    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4 (xcubic.yw, ycubic.yw) / s;
    
    offset *= invTexSize.xxyy;
    
    vec4 sample0 = texture(samp, offset.xz);
    vec4 sample1 = texture(samp, offset.yz);
    vec4 sample2 = texture(samp, offset.xw);
    vec4 sample3 = texture(samp, offset.yw);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(mix(sample3, sample2, sx), mix(sample1, sample0, sx), sy);
}

/*
vec4 textureMagic(sampler2D samp, vec2 uv) {
    vec2 invPixelSize = 1.0/textureSize(samp, 0);

    uv -= 0.5 * invPixelSize;
    vec4 col = vec4(0.0);
    col += 0.37487566 * texture(samp, uv + vec2(-0.75777,-0.75777)*invPixelSize);
    col += 0.37487566 * texture(samp, uv + vec2(0.75777,-0.75777)*invPixelSize);
    col += 0.37487566 * texture(samp, uv + vec2(0.75777,0.75777)*invPixelSize);
    col += 0.37487566 * texture(samp, uv + vec2(-0.75777,0.75777)*invPixelSize);
     
    col += -0.12487566 * texture(samp, uv + vec2(-2.907,0.0)*invPixelSize);
    col += -0.12487566 * texture(samp, uv + vec2(2.907,0.0)*invPixelSize);
    col += -0.12487566 * texture(samp, uv + vec2(0.0,-2.907)*invPixelSize);
    col += -0.12487566 * texture(samp, uv + vec2(0.0,2.907)*invPixelSize);

    return col;
}*/

vec4 textureBilinear(sampler2D samp, vec2 uv)
{
    vec2 ts = textureSize(samp, 0);
    // calculate coordinates -> also need to offset by half a pixel to keep image from shifting down and left half a pixel
    vec2 xy = (uv * ts) - 0.5;

    vec2 pix = floor(xy);

    vec4 idx = (pix.xxyy + vec2(0.5, 1.5).xyxy)/ts.xxyy;

    vec2 f   = fract(xy);
 
    // get pixels
    vec4 p00 = texture(samp, idx.xz);
    vec4 p10 = texture(samp, idx.yz);
    vec4 p01 = texture(samp, idx.xw);
    vec4 p11 = texture(samp, idx.yw);
 
    // interpolate bi-linearly!
    vec4 value = mix(mix(p00, p10, f.x), mix(p01, p11, f.x), f.y);

    return value;
}

/*
vec4 textureBilinear(sampler2D samp, vec2 uv) {
    vec2 textureSize = textureSize(samp, 0);
    vec2 ts = 1.0/textureSize; // size of one pixel in UV space

    vec2 f = fract(uv * textureSize);
    //uv -= (f - 0.5)*ts; // move uv to texel centre
    uv = uv + ts*(0.5 - f);
    vec4 tl = texture(samp, uv);
    vec4 tr = texture(samp, uv + vec2(ts.x, 0.0));
    vec4 bl = texture(samp, uv + vec2(0.0, ts.y));
    vec4 br = texture(samp, uv + vec2(ts.x,ts.y));

    return mix(mix(tl, tr, f.x), mix(bl, br, f.x), f.y);
}*/


void main() {
    vec4 tex;

    /*
    if (PushConstants.toFilter)
        tex = textureBilinear(texSampler, fragTexCoord);
    else*/
        tex = texture(texSampler, fragTexCoord);

    outColor = fragColor * tex;
}

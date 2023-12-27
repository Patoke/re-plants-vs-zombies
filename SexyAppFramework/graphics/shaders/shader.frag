#version 450

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    //outColor = vec4(fragTexCoord, 0.0, 1.0);

    vec2 z = 3*(fragTexCoord - vec2(2.0/3.0,0.5));
    vec2 c = z;

    const int iterations = 100;
    // Iterate
    int i = 0;
    for(; i < iterations; i++) {
        z = vec2(z.x*z.x - z.y*z.y, 2*z.x*z.y) + c;
        if (z.x*z.x + z.y*z.y > 4.0) break;
    }

    if (i == iterations) {
        outColor = vec4(0.0,0.0,0.0,1.0);
    } else {
        const vec4 colours[4] = {
            vec4(1.0,1.0,1.0,1.0),
            vec4(0.5,1.0,0.0,1.0),
            vec4(1.0,0.5,0.0,1.0),
            vec4(0.0,1.0,0.5,1.0),
        };

        outColor = colours[i%4];
    }
}

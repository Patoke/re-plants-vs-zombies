#version 450

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 fragColor;

layout(push_constant) uniform constants {
    vec4 vertices[4];
    uint colors[4];
    vec4 renderColor;
    bool isQuad;
    bool toFilter;
} PushConstants;

int indexBuffer[6] = {
    0, 1, 2, 2, 1, 3
};

void main() {
    bool is_color;
    if (PushConstants.colors[0] == 0 && PushConstants.colors[1] == 0 && PushConstants.colors[2] == 0) {
        is_color = false;
    } else {
        is_color = true;
    }

    //fragTexCoord = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    vec4 vertex;
    if (PushConstants.isQuad) {
        vertex = PushConstants.vertices[indexBuffer[gl_VertexIndex]];
    } else {
        vertex = PushConstants.vertices[gl_VertexIndex];
    }
    
    fragTexCoord = vertex.zw;
    gl_Position  = vec4(vertex.xy, 0.0, 1.0);

    uint c = PushConstants.colors[gl_VertexIndex];
    vec4 outColor = vec4(
        (c & 0xFF)/255.0,
        (c >> 8 & 0xFF)/255.0,
        (c >> 16 & 0xFF)/255.0,
        (c >> 24)/255.0
    );

    if (is_color)
        fragColor = outColor;
    else
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

#version 450

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 fragColor;

layout(push_constant) uniform constants {
    vec4 vertices[4];
    vec4 colors[4];
    bool isQuad;
    bool toFilter;
} PushConstants;

int indexBuffer[6] = {
    0, 1, 2, 2, 1, 3
};

void main() {
    //fragTexCoord = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    uint index;
    if (PushConstants.isQuad) {
        index = indexBuffer[gl_VertexIndex];
    } else {
        index = gl_VertexIndex;
    }

    vec4 vertex  = PushConstants.vertices[index];
    fragColor    = PushConstants.colors[index];
    
    fragTexCoord = vertex.zw;
    gl_Position  = vec4(vertex.xy, 0.0, 1.0);
}

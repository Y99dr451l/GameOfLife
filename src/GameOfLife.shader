#shader vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
out vec2 texCoord;
void main() {
    gl_Position = vec4(aPos, 0.f, 1.f);
    texCoord = aTexCoord;
}

#shader fragment
#version 330 core
out vec4 color;
in vec2 texCoord;
uniform sampler2D tex;
void main() {
    color = texture(tex, texCoord);
    color = 255.f * vec4(color.xxx, 1.f);
}
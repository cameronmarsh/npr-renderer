#version 410

in vec4 vPosition;
in vec2 vTexture;

out vec2 texCoord;
out vec4 color;

void main() {

    gl_Position = vPosition;
    texCoord = (vec2(vPosition.x, vPosition.y) + vec2(1, 1))*0.5;
}
    


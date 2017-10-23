#version 410

uniform sampler2D tex0;

in vec2 texCoord;
out vec4 fragColor;

void main() {      
   fragColor = texture(tex0, texCoord);
}

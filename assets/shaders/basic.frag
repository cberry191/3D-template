#version 410 core
out vec4 FragColor;

uniform vec3 color;
uniform bool isSelected;

void main()
{
    vec3 baseColour = color;
    if (isSelected) {
        baseColour = mix(color, vec3(1.0, 1.0, 0.0), 0.5);
    }
    FragColor = vec4(baseColour, 1.0);
}

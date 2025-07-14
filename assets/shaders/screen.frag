#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, TexCoords).rgb;
    // vec3 color = vec3(TexCoords, 0.0); // visualize UV coords
    FragColor = vec4(color, 1.0);
}

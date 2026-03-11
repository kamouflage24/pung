#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform float u_spacing;
uniform vec4 u_resolution;

uniform vec4 u_gridDark;
uniform vec4 u_gridLight;

uniform vec2 u_offset;

void main()
{
    vec2 uv = TexCoord * u_resolution.xy + u_offset;
    vec2 f = fract(uv / u_spacing);
    float line = step(f.x, 0.5) + step(f.y, 0.5);
    FragColor = mix(u_gridDark, u_gridLight, line);

}
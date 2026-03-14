#version 330 core

out vec4 FragColor; 
in vec2 TexCoord;
uniform vec2 u_resolution;
uniform float u_time;

float random(vec2 st){
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float noise(vec2 st){
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * f*(f*(f*6.0 - 15.0)+10.0);
    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;

}
vec3 fcolor(float t){
   t = clamp(t, 0.0, 1.0);
   if(t < 0.3) return mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.5, 0.0), t/0.3);
   else if(t < 0.6) return mix(vec3(1.0, 0.5, 0.0), vec3(1.0, 0.0, 0.0), (t/0.3)/0.3);
   else return mix(vec3(1.0,0.0,0.0), vec3(0.2,0.0,0.0), (t-0.6)/0.4);
}

void main(){
    vec2 st = vec2(TexCoord.x, 1.0 - TexCoord.y);
    float n = 0.0;
    float scale = 3;
    float speed = 1.0;
    for(int i = 1; i <=3; i++){
        float freq = 1.5 * float(i);
        float amp = 1.1 / float(i);
        n += noise(st * freq + vec2(0.0, u_time * speed * i)) * amp;
    }
   
    vec3 color = fcolor(n);

   
    FragColor = vec4(color, n);
    
}
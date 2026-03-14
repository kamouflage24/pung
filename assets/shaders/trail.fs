#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2d uPrevTrail;
uniform vec2 uBallPos;
uniform float uBallRadius;
uniform vec4 uBallColor;
uniform float uFade;

void main(){
    vec4 prev = texture(uPrevTrail, TecCoord);
    prev.rgb *= prev.a;
    prev.a *= uFade;

    float dist = distance(TexCoord, uBallPos);
    float alpha = smoothstep(uBallRadius, 0.0, dist);

    vec4 ball = vec4(uBallColor.rgb, uBallColor.a * alpha);
    
}
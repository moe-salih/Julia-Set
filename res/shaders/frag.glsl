
#version 330 core

layout (location = 0) out vec4 FragColor;
in vec4 coord;

uniform vec4 world;
uniform vec4 low;
uniform vec4 high;
uniform int  iterations;
uniform int  power;
uniform vec2 mouse;
uniform vec2 resolution;


vec2 eular_pow(vec2 z, int n) {
    float r = length(z);
    float theta = atan(z.y, z.x);

    float new_r = pow(r, n);
    float new_theta = n * theta;

    return vec2(new_r * cos(new_theta), new_r * sin(new_theta));
}

vec4 julia(vec2 z, vec2 c) {
    vec4 color = high;

    for(int i=0; i<iterations; ++i) {
        z = eular_pow(z, power) + c;

        if(length(z) > 2.0) {
            color = mix(low, high, pow(float(i) / float(iterations), 1.2));
            break;
        }
    }
    return color;
}

void main() {
    vec2 z = coord.xy * (world.zw - world.xy) + world.xy;
    vec2 c = mouse;

    if(resolution.x > resolution.y) {
        z.x *= resolution.x / resolution.y;
    } else {
        z.y *= resolution.y / resolution.x;
    }

    FragColor = julia(z, c);

}
